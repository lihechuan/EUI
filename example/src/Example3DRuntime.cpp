#include <QDir>
#include <QProcess>
#include <QSysInfo>
#include <QDateTime>
#include <QFileInfo>
#include <QSaveFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTemporaryDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QProcessEnvironment>

#include "Example3DRuntime.h"

namespace
{
QString cacheRoot()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
        + QStringLiteral("/EUI/Quick3D");
}

bool saveFile(const QString &path, const QByteArray &contents)
{
    QSaveFile file(path);
    return file.open(QIODevice::WriteOnly) && file.write(contents) == contents.size() && file.commit();
}

bool safePath(const QString &path)
{
    if (path.isEmpty() || QDir::isAbsolutePath(path) || path.contains(QLatin1Char('\\'))
        || path.contains(QLatin1Char(':')) || path.contains(QChar::Null))
        return false;
    for (const auto &part : path.split(QLatin1Char('/')))
    {
        if (part.isEmpty() || part == QStringLiteral(".") || part == QStringLiteral("..")
            || part.endsWith(QLatin1Char('.')) || part.endsWith(QLatin1Char(' ')))
            return false;
    }
    return true;
}

bool regularFile(const QString &root, const QString &relative)
{
    QString path = root;
    for (const auto &part : relative.split(QLatin1Char('/')))
    {
        path += QLatin1Char('/') + part;
        if (QFileInfo(path).isSymLink())
            return false;
    }
    return QFileInfo(path).isFile();
}
}

Example3DRuntime::Example3DRuntime(QObject *parent, const QString &payloadDirectory)
    : QObject(parent), m_busy(false), m_dark(false), m_index(0), m_cached(false), m_total(0),
      m_stage(QStringLiteral("idle")), m_installed(false), m_progress(0), m_skipCache(false), m_processed(0),
      m_staging(nullptr), m_hash(QCryptographicHash::Sha256), m_payloadDirectory(payloadDirectory)
{
    connect(&m_timer, &QTimer::timeout, this, &Example3DRuntime::copyNext);
    m_deadline.setSingleShot(true);
    connect(&m_deadline, &QTimer::timeout, this, [this]() { fail(QStringLiteral("launchTimeout")); });
    const auto arguments = QCoreApplication::arguments();
    const int index = arguments.indexOf(QStringLiteral("--quick3d-ready-file"));
    if (index >= 0 && index + 1 < arguments.size())
        m_readyFile = arguments.at(index + 1);
    if (m_readyFile.isEmpty())
        locatePayload();
}

Example3DRuntime::~Example3DRuntime()
{
    if (m_child)
    {
        if (m_stage == QStringLiteral("preparing"))
            m_child->terminate();
        else
            m_child->kill();
        if (!m_child->waitForFinished(3000))
            m_child->kill();
    }
    delete m_staging;
}

bool Example3DRuntime::busy() const { return m_busy; }
QString Example3DRuntime::error() const { return m_error; }
QString Example3DRuntime::stage() const { return m_stage; }
qreal Example3DRuntime::progress() const { return m_progress; }
bool Example3DRuntime::installed() const { return m_installed; }

bool Example3DRuntime::supported() const
{
#ifdef Q_OS_WIN
    return QSysInfo::currentCpuArchitecture() == QStringLiteral("x86_64");
#elif defined(Q_OS_MACOS)
    return QSysInfo::currentCpuArchitecture() == QStringLiteral("x86_64") ||
           QSysInfo::currentCpuArchitecture() == QStringLiteral("arm64");
#else
    return false;
#endif
}

QString Example3DRuntime::platform() const
{
#ifdef Q_OS_MACOS
    return QStringLiteral("macOS");
#elif defined(Q_OS_WIN)
    return QStringLiteral("Windows");
#else
    return QSysInfo::productType();
#endif
}

void Example3DRuntime::confirmReady()
{
    if (m_readyFile.isEmpty())
        return;
    QSaveFile file(m_readyFile);
    if (file.open(QIODevice::WriteOnly) && file.write("ready") == 5 && file.commit())
    {
        m_readyFile.clear();
        if (QCoreApplication::arguments().contains(QStringLiteral("--quick3d-runtime-check")))
            QTimer::singleShot(1000, QCoreApplication::instance(), &QCoreApplication::quit);
    }
}

void Example3DRuntime::fail(const QString &error)
{
    m_timer.stop();
    m_deadline.stop();
    m_input.close();
    m_output.close();
    if (m_cached && m_source != m_destination
        && (error == QStringLiteral("readFailed") || error == QStringLiteral("hashMismatch")))
    {
        m_busy = false;
        m_skipCache = true;
        m_installed = false;
        install(m_language, m_dark);
        return;
    }
    m_error = error;
    m_busy = false;
    m_stage = QStringLiteral("failed");
    if (m_child)
    {
        m_child->disconnect(this);
        m_child->kill();
        m_child->deleteLater();
        m_child = nullptr;
    }
    delete m_staging;
    m_staging = nullptr;
    emit changed();
}

bool Example3DRuntime::readManifest(const QString &directory)
{
    QFile file(directory + QStringLiteral("/manifest.json"));
    if (QFileInfo(file).isSymLink() || !file.open(QIODevice::ReadOnly) || file.size() > 4 * 1024 * 1024)
        return false;
    m_manifest = file.readAll();
    const auto document = QJsonDocument::fromJson(m_manifest);
    const auto manifest = document.object();
    m_files             = manifest.value(QStringLiteral("files")).toArray();
#ifdef Q_OS_MACOS
    const auto entry = QStringLiteral("EUI Gallery.app/Contents/MacOS/example");
    const auto architecture =
        QSysInfo::currentCpuArchitecture() == QStringLiteral("arm64") ? QStringLiteral("arm64") : QStringLiteral("x64");
    if (manifest.value(QStringLiteral("platform")).toString() != QStringLiteral("macos"))
        return false;
#else
    const auto entry        = QStringLiteral("EUI-Gallery-Qt6.exe");
    const auto architecture = QStringLiteral("x64");
#endif
    if (!document.isObject() || manifest.value(QStringLiteral("schema")).toInt() != 1 ||
        manifest.value(QStringLiteral("architecture")).toString() != architecture ||
        manifest.value(QStringLiteral("entry")).toString() != entry || m_files.isEmpty() || m_files.size() > 10000)
        return false;
    QStringList paths;
    m_total = 0;
    for (const auto &value : m_files)
    {
        const auto record = value.toObject();
        const auto path = record.value(QStringLiteral("path")).toString();
        const auto size = record.value(QStringLiteral("size")).toDouble(-1);
        const auto hash = record.value(QStringLiteral("sha256")).toString().toLatin1();
        if (!safePath(path) || paths.contains(path, Qt::CaseInsensitive) || size < 0 || size > 536870912
            || size != static_cast<qint64>(size) || hash.size() != 64
            || QByteArray::fromHex(hash).toHex() != hash)
            return false;
#ifdef Q_OS_MACOS
        const auto link = record.value(QStringLiteral("link")).toString();
        if (!path.startsWith(QStringLiteral("EUI Gallery.app/")))
            return false;
        if (!link.isEmpty())
        {
            const auto target = QDir::cleanPath(QFileInfo(path).path() + QLatin1Char('/') + link);
            if (QDir::isAbsolutePath(link) || link.contains(QLatin1Char(':')) || link.contains(QLatin1Char('\\')) ||
                link.contains(QChar::Null) || !target.startsWith(QStringLiteral("EUI Gallery.app/")) || size != 0 ||
                hash != QCryptographicHash::hash({}, QCryptographicHash::Sha256).toHex())
                return false;
        }
#else
        if (record.contains(QStringLiteral("link")))
            return false;
#endif
        paths.append(path);
        m_total += static_cast<qint64>(size);
    }
    return m_total > 0 && m_total < 2147483648LL
#ifdef Q_OS_MACOS
           && paths.contains(entry) && paths.contains(QStringLiteral("EUI Gallery.app/Contents/Resources/qt.conf")) &&
           paths.contains(QStringLiteral("EUI Gallery.app/Contents/Frameworks/QtCore.framework/Versions/A/QtCore")) &&
           paths.contains(
               QStringLiteral("EUI Gallery.app/Contents/Frameworks/QtQuick3D.framework/Versions/A/QtQuick3D"));
#else
           && paths.contains(QStringLiteral("EUI-Gallery-Qt6.exe")) && paths.contains(QStringLiteral("Qt6Core.dll")) &&
           paths.contains(QStringLiteral("Qt6Quick3D.dll")) && paths.contains(QStringLiteral("qt.conf"));
#endif
}

QByteArray Example3DRuntime::cacheFingerprint() const
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(m_manifest);
    for (const auto &value : m_files)
    {
        const auto record = value.toObject();
        const auto path = record.value(QStringLiteral("path")).toString();
        const QFileInfo file(m_destination + QLatin1Char('/') + path);
#ifdef Q_OS_MACOS
        const auto link = record.value(QStringLiteral("link")).toString();
        if (!link.isEmpty())
        {
            if (!file.isSymLink() || file.symLinkTarget() != QDir::cleanPath(file.path() + QLatin1Char('/') + link) ||
                !file.canonicalFilePath().startsWith(m_destination + QStringLiteral("/EUI Gallery.app/")))
                return {};
            hash.addData(link.toUtf8());
            continue;
        }
        if (file.isExecutable() != record.value(QStringLiteral("executable")).toBool())
            return {};
#endif
        if (!regularFile(m_destination, path) ||
            file.size() != static_cast<qint64>(record.value(QStringLiteral("size")).toDouble()))
            return {};
        hash.addData(QByteArray::number(file.lastModified().toMSecsSinceEpoch()));
        hash.addData("\n");
    }
    return hash.result().toHex();
}

bool Example3DRuntime::cacheVerified() const
{
    QFile receipt(m_destination + QStringLiteral("/.installed"));
    if (!regularFile(m_destination, QStringLiteral(".installed"))
        || !receipt.open(QIODevice::ReadOnly) || receipt.size() != 64)
        return false;
    return receipt.readAll() == cacheFingerprint();
}

bool Example3DRuntime::saveInstallation()
{
    const auto fingerprint = cacheFingerprint();
    const auto digest = QCryptographicHash::hash(m_manifest, QCryptographicHash::Sha256).toHex();
    return fingerprint.size() == 64 && saveFile(m_destination + QStringLiteral("/manifest.json"), m_manifest)
        && saveFile(m_destination + QStringLiteral("/.installed"), fingerprint)
        && saveFile(cacheRoot() + QStringLiteral("/current"), digest);
}

QString Example3DRuntime::locatePayload()
{
    m_installed = false;
    QStringList candidates{QCoreApplication::applicationDirPath() + QStringLiteral("/quick3d-runtime")};
#ifdef EUI_QUICK3D_PAYLOAD_DIR
    candidates.append(QStringLiteral(EUI_QUICK3D_PAYLOAD_DIR));
#endif
    if (!m_payloadDirectory.isEmpty())
        candidates = QStringList{m_payloadDirectory};
    else
    {
        QFile current(cacheRoot() + QStringLiteral("/current"));
        if (!QFileInfo(current).isSymLink() && current.open(QIODevice::ReadOnly) && current.size() == 64)
        {
            const auto digest = current.readAll();
            if (QByteArray::fromHex(digest).toHex() == digest)
                candidates.append(cacheRoot() + QLatin1Char('/') + QString::fromLatin1(digest));
        }
    }
    m_source.clear();
    for (const auto &candidate : candidates)
    {
        if (!QFileInfo::exists(candidate + QStringLiteral("/manifest.json")))
            continue;
        if (!readManifest(candidate))
            return QStringLiteral("invalidManifest");
        const auto manifest = QJsonDocument::fromJson(m_manifest).object();
        if (manifest.value(QStringLiteral("galleryVersion")).toString() != QStringLiteral(EUI_GALLERY_VERSION)
            || manifest.value(QStringLiteral("galleryRevision")).toString() != QStringLiteral(EUI_GALLERY_REVISION))
            return QStringLiteral("runtimeMismatch");
        m_source = QFileInfo(candidate).absoluteFilePath();
        break;
    }
    if (m_source.isEmpty())
        return QStringLiteral("missingPayload");
    const auto root = cacheRoot();
    if (QFileInfo(root).isSymLink())
        return QStringLiteral("writeFailed");
    const auto digest = QCryptographicHash::hash(m_manifest, QCryptographicHash::Sha256).toHex();
    m_destination = root + QLatin1Char('/') + QString::fromLatin1(digest);
    if (QFileInfo(m_destination).isSymLink())
        return QStringLiteral("writeFailed");
    m_installed = !cacheFingerprint().isEmpty();
    return {};
}

void Example3DRuntime::install(const QString &language, bool dark)
{
    if (m_busy)
        return;
    m_busy = true;
    m_error.clear();
    m_progress = 0;
    m_stage = QStringLiteral("checking");
    m_language = language == QStringLiteral("zh") ? language : QStringLiteral("en");
    m_dark = dark;
    emit changed();
    if (!supported())
    {
        fail(QStringLiteral("unsupported"));
        return;
    }
    const auto error = locatePayload();
    if (!error.isEmpty())
    {
#if defined(Q_OS_MACOS) && defined(EUI_QUICK3D_PREPARE_SCRIPT)
        if (m_payloadDirectory.isEmpty() &&
            (error == QStringLiteral("missingPayload") || error == QStringLiteral("runtimeMismatch")) &&
            QFileInfo::exists(QStringLiteral(EUI_QUICK3D_PREPARE_SCRIPT)))
        {
            prepareRuntime();
            return;
        }
#endif
        fail(error);
        return;
    }
    if (!QDir().mkpath(cacheRoot()))
    {
        fail(QStringLiteral("writeFailed"));
        return;
    }
    if (!m_skipCache && cacheVerified())
    {
        launch();
        return;
    }
    m_cached = !m_skipCache && QDir(m_destination).exists();
    if (!m_cached)
    {
        delete m_staging;
        m_staging = new QTemporaryDir(cacheRoot() + QStringLiteral("/.install-XXXXXX"));
        if (!m_staging->isValid())
        {
            fail(QStringLiteral("writeFailed"));
            return;
        }
    }
    m_index = 0;
    m_processed = 0;
    m_stage = m_cached ? QStringLiteral("verifying") : QStringLiteral("installing");
    emit changed();
    m_timer.start(0);
}

void Example3DRuntime::prepareRuntime()
{
#if defined(Q_OS_MACOS) && defined(EUI_QUICK3D_PREPARE_SCRIPT)
    const auto python = QStandardPaths::findExecutable(QStringLiteral("python3"));
    if (python.isEmpty())
    {
        fail(QStringLiteral("prepareFailed"));
        return;
    }
    m_stage = QStringLiteral("preparing");
    emit changed();
    m_child          = new QProcess(this);
    auto environment = QProcessEnvironment::systemEnvironment();
    for (const auto &name : environment.keys())
        if (name.startsWith(QStringLiteral("QT_")) || name.startsWith(QStringLiteral("QML")) ||
            name.startsWith(QStringLiteral("DYLD_")))
            environment.remove(name);
    m_child->setProcessEnvironment(environment);
    m_child->setProcessChannelMode(QProcess::ForwardedChannels);
    connect(m_child, &QProcess::errorOccurred, this, [this]() { fail(QStringLiteral("prepareFailed")); });
    connect(m_child,
            qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
            this,
            [this](int code, QProcess::ExitStatus status)
            {
                if (code != 0 || status != QProcess::NormalExit)
                {
                    fail(QStringLiteral("prepareFailed"));
                    return;
                }
                m_child->deleteLater();
                m_child = nullptr;
                m_busy  = false;
                install(m_language, m_dark);
            });
    m_child->start(python,
                   {QStringLiteral(EUI_QUICK3D_PREPARE_SCRIPT),
                    QStringLiteral("--install-qt"),
                    QStringLiteral("--gallery-version"),
                    QStringLiteral(EUI_GALLERY_VERSION),
                    QStringLiteral("--gallery-revision"),
                    QStringLiteral(EUI_GALLERY_REVISION)});
#else
    fail(QStringLiteral("unsupported"));
#endif
}

void Example3DRuntime::copyNext()
{
    if (m_stage == QStringLiteral("launching"))
    {
        QFile file(m_readyFile);
        if (file.open(QIODevice::ReadOnly) && file.readAll() == "ready")
        {
            m_timer.stop();
            m_deadline.stop();
            QFile::remove(m_readyFile);
            m_progress = 1;
            m_busy = false;
            m_stage = QStringLiteral("ready");
            // The successor must survive destruction of the Qt 5 application during handoff.
            m_child->disconnect(this);
            m_child->setParent(nullptr);
            m_child = nullptr;
            emit changed();
            emit handoffReady();
        }
        return;
    }
    if (m_index >= m_files.size())
    {
        m_timer.stop();
        if (!m_cached)
        {
#ifdef Q_OS_MACOS
            // Create bundle links only after file writes, so installation cannot follow them outside the staging
            // directory.
            for (const auto &value : m_files)
            {
                const auto record = value.toObject();
                const auto link   = record.value(QStringLiteral("link")).toString();
                if (link.isEmpty())
                    continue;
                const auto destination =
                    m_staging->path() + QLatin1Char('/') + record.value(QStringLiteral("path")).toString();
                if (!QDir().mkpath(QFileInfo(destination).path()) || !QFile::link(link, destination))
                {
                    fail(QStringLiteral("writeFailed"));
                    return;
                }
            }
#endif
            if (QFileInfo(m_destination).isSymLink() ||
                (QDir(m_destination).exists() && !QDir(m_destination).removeRecursively()) ||
                !QDir().rename(m_staging->path(), m_destination))
            {
                fail(QStringLiteral("writeFailed"));
                return;
            }
            m_staging->setAutoRemove(false);
            delete m_staging;
            m_staging = nullptr;
        }
        if (!saveInstallation())
        {
            fail(QStringLiteral("writeFailed"));
            return;
        }
        m_skipCache = false;
        m_installed = true;
        launch();
        return;
    }
    const auto record = m_files.at(m_index).toObject();
    const auto relative = record.value(QStringLiteral("path")).toString();
#ifdef Q_OS_MACOS
    if (!record.value(QStringLiteral("link")).toString().isEmpty())
    {
        ++m_index;
        return;
    }
#endif
    if (!m_input.isOpen())
    {
        const auto origin = m_cached ? m_destination : m_source;
        m_input.setFileName(origin + QLatin1Char('/') + relative);
        if (!regularFile(origin, relative) || !m_input.open(QIODevice::ReadOnly)
            || m_input.size() != static_cast<qint64>(record.value(QStringLiteral("size")).toDouble()))
        {
            m_skipCache = true;
            fail(QStringLiteral("readFailed"));
            return;
        }
        m_hash.reset();
        if (!m_cached)
        {
            m_output.setFileName(m_staging->path() + QLatin1Char('/') + relative);
            if (!QDir().mkpath(QFileInfo(m_output).absolutePath()) || !m_output.open(QIODevice::WriteOnly))
            {
                fail(QStringLiteral("writeFailed"));
                return;
            }
        }
    }
    const auto block = m_input.read(1024 * 1024);
    if (m_input.error() != QFileDevice::NoError || (!m_cached && m_output.write(block) != block.size()))
    {
        fail(QStringLiteral("writeFailed"));
        return;
    }
    m_hash.addData(block);
    m_processed += block.size();
    m_progress = 0.9 * static_cast<qreal>(m_processed) / m_total;
    emit changed();
    if (!m_input.atEnd())
        return;
    m_input.close();
#ifdef Q_OS_MACOS
    if (!m_cached)
    {
        auto permissions = QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther;
        if (record.value(QStringLiteral("executable")).toBool())
            permissions |= QFile::ExeOwner | QFile::ExeGroup | QFile::ExeOther;
        if (!m_output.setPermissions(permissions))
        {
            fail(QStringLiteral("writeFailed"));
            return;
        }
    }
#endif
    m_output.close();
    if (m_hash.result().toHex() != record.value(QStringLiteral("sha256")).toString().toLatin1())
    {
        m_skipCache = true;
        fail(QStringLiteral("hashMismatch"));
        return;
    }
    ++m_index;
}

void Example3DRuntime::launch()
{
    auto environment = QProcessEnvironment::systemEnvironment();
    for (const auto &name : environment.keys())
    {
        if (name.startsWith(QStringLiteral("QT_")) || name.startsWith(QStringLiteral("QML")) ||
            name.startsWith(QStringLiteral("QSG_")) || name.startsWith(QStringLiteral("QTWEBENGINE_")) ||
            name.startsWith(QStringLiteral("DYLD_")))
            environment.remove(name);
    }
    environment.insert(QStringLiteral("QT_QUICK_CONTROLS_STYLE"), QStringLiteral("Basic"));
#ifdef Q_OS_MACOS
    environment.insert(QStringLiteral("PATH"), QStringLiteral("/usr/bin:/bin:/usr/sbin:/sbin"));
    environment.insert(QStringLiteral("QSG_RHI_BACKEND"), QStringLiteral("metal"));
#else
    environment.insert(QStringLiteral("PATH"),
                       m_destination + QLatin1Char(';') + qEnvironmentVariable("WINDIR") + QStringLiteral("/System32"));
    environment.insert(QStringLiteral("QSG_RHI_BACKEND"), QStringLiteral("d3d11"));
#endif
    environment.insert(QStringLiteral("EUI_GALLERY_INSTALL_DIR"),
        qEnvironmentVariable("EUI_GALLERY_INSTALL_DIR", QCoreApplication::applicationDirPath()));
    m_readyFile = m_destination + QStringLiteral("/.ready-") + QString::number(QCoreApplication::applicationPid());
    QFile::remove(m_readyFile);
    m_child = new QProcess(this);
    if (QCoreApplication::arguments().contains(QStringLiteral("--quick3d-runtime-check")))
    {
        environment.insert(QStringLiteral("QT_FORCE_STDERR_LOGGING"), QStringLiteral("1"));
        if (qEnvironmentVariable("QT_QPA_PLATFORM") == QStringLiteral("offscreen"))
            environment.insert(QStringLiteral("QT_QPA_PLATFORM"), QStringLiteral("offscreen"));
        m_child->setProcessChannelMode(QProcess::ForwardedChannels);
    }
    m_child->setProcessEnvironment(environment);
    m_child->setWorkingDirectory(m_destination);
    QStringList arguments{QStringLiteral("--quick3d"), QStringLiteral("--language"), m_language,
        QStringLiteral("--quick3d-ready-file"), m_readyFile};
    if (m_dark)
        arguments.append(QStringLiteral("--dark"));
    if (QCoreApplication::arguments().contains(QStringLiteral("--quick3d-runtime-check")))
        arguments.append(QStringLiteral("--quick3d-runtime-check"));
    connect(m_child, &QProcess::started, this, [this]() {
        if (QCoreApplication::arguments().contains(QStringLiteral("--quick3d-runtime-check")))
            qInfo("QUICK3D_CHILD %lld", static_cast<long long>(m_child->processId()));
    });
    connect(m_child, &QProcess::errorOccurred, this, [this]() { fail(QStringLiteral("launchFailed")); });
    connect(m_child, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [this]() {
        if (m_busy && m_stage == QStringLiteral("launching"))
            fail(QStringLiteral("launchFailed"));
    });
    m_stage = QStringLiteral("launching");
    m_progress = 0.95;
    emit changed();
    m_timer.start(100);
    const auto entry = QJsonDocument::fromJson(m_manifest).object().value(QStringLiteral("entry")).toString();
    m_child->start(m_destination + QLatin1Char('/') + entry, arguments);
    m_deadline.start(45000);
}
