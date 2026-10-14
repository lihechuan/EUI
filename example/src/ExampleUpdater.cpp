#include <QDir>
#include <QProcess>
#include <QSysInfo>
#include <QFileInfo>
#include <QLockFile>
#include <QJsonArray>
#include <QSslSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QTemporaryDir>
#include <QVersionNumber>
#include <QNetworkRequest>
#include <QCoreApplication>
#include <QRegularExpression>

#include "ExampleUpdater.h"

namespace
{
QVersionNumber releaseVersion(QString value)
{
    if (value.startsWith(QLatin1Char('v')))
        value.remove(0, 1);
    if (!QRegularExpression(QStringLiteral("^[0-9]+(\\.[0-9]+){1,3}$")).match(value).hasMatch())
        return {};
    int suffix = 0;
    const auto version = QVersionNumber::fromString(value, &suffix);
    return suffix == value.size() ? version : QVersionNumber();
}

bool writeFile(const QString &path, const QByteArray &content)
{
    QFile file(path);
    return file.open(QIODevice::WriteOnly) && file.write(content) == content.size() && file.flush();
}
}

ExampleUpdater::ExampleUpdater(QObject *parent, const QString &directory)
    : QObject(parent), m_dark(false), m_stage(QStringLiteral("idle")), m_progress(0), m_lock(nullptr),
      m_releaseUrl(QStringLiteral("https://api.github.com/repos/lihechuan/EUI/releases/latest")), m_assetSize(0),
      m_directory(directory.isEmpty() ? QCoreApplication::applicationDirPath() : directory),
      m_hash(QCryptographicHash::Sha256), m_workspace(nullptr)
{
#ifdef Q_OS_MACOS
    if (directory.isEmpty())
        m_directory = QDir(m_directory + QStringLiteral("/../..")).absolutePath();
#endif
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this]() { fail(QStringLiteral("timeout")); });
}

ExampleUpdater::~ExampleUpdater()
{
    if (m_reply)
    {
        m_reply->disconnect(this);
        m_reply->abort();
    }
    if (m_process)
    {
        m_process->disconnect(this);
        m_process->kill();
        m_process->waitForFinished(3000);
    }
    m_file.close();
    delete m_workspace;
    delete m_lock;
}

bool ExampleUpdater::busy() const
{
    return m_stage == QStringLiteral("checking") || m_stage == QStringLiteral("checksums")
        || m_stage == QStringLiteral("downloading") || m_stage == QStringLiteral("verifying")
        || m_stage == QStringLiteral("extracting") || m_stage == QStringLiteral("restarting");
}

QString ExampleUpdater::error() const { return m_error; }
QString ExampleUpdater::stage() const { return m_stage; }
qreal ExampleUpdater::progress() const { return m_progress; }
QString ExampleUpdater::latestVersion() const { return m_latestVersion; }
QString ExampleUpdater::currentVersion() const { return QStringLiteral(EUI_GALLERY_VERSION); }

void ExampleUpdater::reportRollback()
{
    fail(QStringLiteral("rollback"));
}

void ExampleUpdater::setStage(const QString &stage, qreal progress)
{
    m_stage = stage;
    m_progress = progress;
    emit changed();
}

void ExampleUpdater::fail(const QString &error)
{
    m_timer.stop();
    m_error = error;
    if (m_reply)
    {
        m_reply->disconnect(this);
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    if (m_process)
    {
        m_process->disconnect(this);
        m_process->kill();
        m_process->waitForFinished(3000);
        m_process->deleteLater();
        m_process = nullptr;
    }
    m_file.close();
    delete m_workspace;
    m_workspace = nullptr;
    delete m_lock;
    m_lock = nullptr;
    setStage(QStringLiteral("failed"));
}

QString ExampleUpdater::assetName() const
{
    QString platform;
    QString architecture = QSysInfo::buildCpuArchitecture();
#ifdef Q_OS_WIN
    platform = QStringLiteral("windows");
    QFile executable(m_directory + QStringLiteral("/EUI-Gallery.exe"));
    if (executable.open(QIODevice::ReadOnly))
    {
        const auto header = executable.read(64);
        if (header.size() == 64 && header.startsWith("MZ"))
        {
            quint32 offset = 0;
            for (int index = 0; index < 4; ++index)
                offset |= static_cast<quint32>(static_cast<uchar>(header.at(60 + index))) << (8 * index);
            if (offset < 1024 * 1024 && executable.seek(offset))
            {
                const auto signature = executable.read(6);
                if (signature == QByteArray("PE\0\0\x4c\x01", 6))
                    architecture = QStringLiteral("i386");
                else if (signature == QByteArray("PE\0\0\x64\x86", 6))
                    architecture = QStringLiteral("x86_64");
            }
        }
    }
#elif defined(Q_OS_MACOS)
    platform = QStringLiteral("macos");
#else
    return {};
#endif
    const auto arch = architecture == QStringLiteral("x86_64") ? QStringLiteral("x64")
        : architecture == QStringLiteral("i386") ? QStringLiteral("x86")
        : architecture == QStringLiteral("arm64") ? QStringLiteral("arm64") : QString();
    if (arch.isEmpty() || (platform == QStringLiteral("windows") && arch == QStringLiteral("arm64")))
        return {};
    return QStringLiteral("EUI-Gallery-%1-%2-%3-portable.zip").arg(m_latestVersion, platform, arch);
}

bool ExampleUpdater::trustedUrl(const QUrl &url) const
{
    if (m_releaseUrl.host() == QStringLiteral("127.0.0.1"))
        return url.scheme() == m_releaseUrl.scheme() && url.host() == m_releaseUrl.host()
            && url.port() == m_releaseUrl.port();
    return url.scheme() == QStringLiteral("https") && url.host() == QStringLiteral("github.com")
        && url.userInfo().isEmpty() && url.port(443) == 443 && !url.hasQuery() && !url.hasFragment()
        && url.path().startsWith(QStringLiteral("/lihechuan/EUI/releases/download/v") + m_latestVersion + QLatin1Char('/'));
}

void ExampleUpdater::check()
{
    if (busy())
        return;
    m_error.clear();
    m_assetUrl.clear();
    m_sumsUrl.clear();
    m_latestVersion.clear();
    setStage(QStringLiteral("checking"));
    request(m_releaseUrl);
}

void ExampleUpdater::request(const QUrl &url, bool download)
{
    if (url.scheme() == QStringLiteral("https") && !QSslSocket::supportsSsl())
    {
        fail(QStringLiteral("tlsUnavailable"));
        return;
    }
    m_response.clear();
    QNetworkRequest networkRequest(url);
    networkRequest.setRawHeader("User-Agent", "EUI-Gallery/" EUI_GALLERY_VERSION);
    networkRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    networkRequest.setTransferTimeout(30000);
    m_reply = m_network.get(networkRequest);
    m_reply->setReadBufferSize(1024 * 1024);
    m_timer.start(30000);
    connect(m_reply, &QNetworkReply::readyRead, this, [this, download]() {
        const auto block = m_reply->readAll();
        m_timer.start(30000);
        if (download)
        {
            if (m_file.size() + block.size() > m_assetSize || m_file.write(block) != block.size())
            {
                fail(QStringLiteral("writeFailed"));
                return;
            }
            m_hash.addData(block);
            m_progress = 0.05 + 0.8 * static_cast<qreal>(m_file.size()) / m_assetSize;
            emit changed();
        }
        else
        {
            m_response.append(block);
            if (m_response.size() > 2 * 1024 * 1024)
                fail(QStringLiteral("invalidRelease"));
        }
    });
    connect(m_reply, &QNetworkReply::finished, this, [this, download]() {
        const auto error = m_reply->error();
        const auto status = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        m_reply->deleteLater();
        m_reply = nullptr;
        m_timer.stop();
        if (error != QNetworkReply::NoError || status != 200)
        {
            fail(QStringLiteral("networkFailed"));
            return;
        }
        if (download)
        {
            setStage(QStringLiteral("verifying"), 0.87);
            if (!m_file.flush() || m_file.size() != m_assetSize || m_hash.result().toHex() != m_checksum)
            {
                fail(QStringLiteral("hashMismatch"));
                return;
            }
            m_file.close();
            prepare();
            return;
        }
        if (m_stage == QStringLiteral("checksums"))
        {
            m_checksum.clear();
            for (const auto &line : m_response.split('\n'))
            {
                const auto parts = line.trimmed().split(' ');
                if (parts.size() < 2 || parts.last() != m_assetName.toUtf8())
                    continue;
                const auto checksum = parts.first().toLower();
                if (!m_checksum.isEmpty() || checksum.size() != 64 || QByteArray::fromHex(checksum).toHex() != checksum)
                {
                    fail(QStringLiteral("invalidChecksum"));
                    return;
                }
                m_checksum = checksum;
            }
            if (m_checksum.isEmpty())
            {
                fail(QStringLiteral("invalidChecksum"));
                return;
            }
            m_file.setFileName(m_workspace->filePath(QStringLiteral("package.zip")));
            if (!m_file.open(QIODevice::WriteOnly))
            {
                fail(QStringLiteral("writeFailed"));
                return;
            }
            m_hash.reset();
            setStage(QStringLiteral("downloading"), 0.05);
            request(m_assetUrl, true);
            return;
        }
        const auto document = QJsonDocument::fromJson(m_response);
        const auto release = document.object();
        const auto version = releaseVersion(release.value(QStringLiteral("tag_name")).toString());
        if (!document.isObject() || version.isNull() || release.value(QStringLiteral("draft")).toBool()
            || release.value(QStringLiteral("prerelease")).toBool())
        {
            fail(QStringLiteral("invalidRelease"));
            return;
        }
        m_latestVersion = version.toString();
        if (QVersionNumber::compare(version, releaseVersion(currentVersion())) <= 0)
        {
            setStage(QStringLiteral("current"));
            return;
        }
        m_assetName = assetName();
        if (m_assetName.isEmpty())
        {
            fail(QStringLiteral("unsupported"));
            return;
        }
        for (const auto &value : release.value(QStringLiteral("assets")).toArray())
        {
            const auto asset = value.toObject();
            const auto name = asset.value(QStringLiteral("name")).toString();
            const QUrl url(asset.value(QStringLiteral("browser_download_url")).toString());
            if (!trustedUrl(url) || url.fileName() != name)
                continue;
            if (name == m_assetName)
            {
                const auto size = asset.value(QStringLiteral("size")).toDouble();
                if (!m_assetUrl.isEmpty() || size <= 0 || size > 4294967296.0 || size != static_cast<qint64>(size))
                {
                    fail(QStringLiteral("invalidRelease"));
                    return;
                }
                m_assetUrl = url;
                m_assetSize = static_cast<qint64>(size);
            }
            else if (name == QStringLiteral("SHA256SUMS.txt"))
                m_sumsUrl = url;
        }
        if (m_assetUrl.isEmpty() || m_sumsUrl.isEmpty())
        {
            m_assetUrl.clear();
            fail(QStringLiteral("missingPackage"));
            return;
        }
        setStage(QStringLiteral("available"));
    });
}

void ExampleUpdater::install(const QString &language, bool dark)
{
    if (busy() || m_assetUrl.isEmpty() || m_sumsUrl.isEmpty())
        return;
    const QFileInfo target(m_directory);
    if (target.isSymLink() || !target.isDir()
#ifdef Q_OS_WIN
        || !QFileInfo::exists(m_directory + QStringLiteral("/EUI-Gallery.exe"))
#elif defined(Q_OS_MACOS)
        || !m_directory.endsWith(QStringLiteral(".app"))
        || !QFileInfo::exists(m_directory + QStringLiteral("/Contents/MacOS/example"))
#else
        || true
#endif
    )
    {
        fail(QStringLiteral("developmentBuild"));
        return;
    }
    m_error.clear();
    m_dark = dark;
    m_language = language == QStringLiteral("en") ? language : QStringLiteral("zh");
    m_lock = new QLockFile(target.dir().filePath(QStringLiteral(".eui-gallery-update.lock")));
    if (!m_lock->tryLock())
    {
        fail(QStringLiteral("updateBusy"));
        return;
    }
    m_workspace = new QTemporaryDir(target.dir().filePath(QStringLiteral(".eui-update-XXXXXX")));
    if (!m_workspace->isValid())
    {
        fail(QStringLiteral("writeFailed"));
        return;
    }
    setStage(QStringLiteral("checksums"));
    request(m_sumsUrl);
}

void ExampleUpdater::prepare()
{
    QString program;
    QString resource;
    QStringList arguments;
#ifdef Q_OS_WIN
    program = qEnvironmentVariable("WINDIR") + QStringLiteral("/System32/WindowsPowerShell/v1.0/powershell.exe");
    resource = QStringLiteral(":/example/update/Update.ps1");
    arguments = QStringList{QStringLiteral("-NoProfile"), QStringLiteral("-NonInteractive"), QStringLiteral("-ExecutionPolicy"),
        QStringLiteral("Bypass"), QStringLiteral("-File")};
#elif defined(Q_OS_MACOS)
    program = QStringLiteral("/bin/sh");
    resource = QStringLiteral(":/example/update/Update.sh");
#endif
    const auto script = m_workspace->filePath(QFileInfo(resource).fileName());
    if (program.isEmpty() || !QFile::copy(resource, script))
    {
        fail(QStringLiteral("prepareFailed"));
        return;
    }
    const auto job = m_workspace->filePath(QStringLiteral("job.json"));
    QStringList restartArguments{QStringLiteral("--update-result"), job, QStringLiteral("--language"), m_language};
    if (m_dark)
        restartArguments << QStringLiteral("--dark");
    const auto content = QJsonDocument(QJsonObject{{QStringLiteral("target"), QFileInfo(m_directory).absoluteFilePath()},
        {QStringLiteral("pid"), QCoreApplication::applicationPid()}, {QStringLiteral("version"), m_latestVersion},
        {QStringLiteral("sha256"), QString::fromLatin1(m_checksum)},
        {QStringLiteral("arguments"), QJsonArray::fromStringList(restartArguments)}}).toJson();
    if (!writeFile(job, content))
    {
        fail(QStringLiteral("writeFailed"));
        return;
    }
    arguments << script << QStringLiteral("prepare") << job;
#ifdef Q_OS_MACOS
    arguments << m_directory << QString::number(QCoreApplication::applicationPid()) << m_latestVersion
        << QString::fromLatin1(m_checksum)
        << m_language << (m_dark ? QStringLiteral("dark") : QStringLiteral("light"));
#endif
    m_process = new QProcess(this);
#ifdef Q_OS_WIN
    m_process->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->flags |= 0x08000000;
    });
#endif
    m_response.clear();
    connect(m_process, &QProcess::readyReadStandardOutput, this, [this]() {
        m_response.append(m_process->readAllStandardOutput());
        while (m_response.contains('\n'))
        {
            const auto index = m_response.indexOf('\n');
            const auto line = m_response.left(index).trimmed();
            m_response.remove(0, index + 1);
            bool ok = false;
            const auto percentage = line.toInt(&ok);
            if (ok && percentage >= 0 && percentage <= 100)
            {
                m_progress = 0.9 + 0.07 * percentage / 100;
                m_timer.start(120000);
                emit changed();
            }
        }
    });
    connect(m_process, &QProcess::errorOccurred, this, [this]() { fail(QStringLiteral("prepareFailed")); });
    connect(m_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this,
        [this, program, script, job, arguments](int code, QProcess::ExitStatus status) mutable {
        m_timer.stop();
        m_process->deleteLater();
        m_process = nullptr;
        if (code != 0 || status != QProcess::NormalExit)
        {
            fail(QStringLiteral("prepareFailed"));
            return;
        }
        arguments[arguments.indexOf(QStringLiteral("prepare"))] = QStringLiteral("apply");
        QProcess helper;
        helper.setProgram(program);
        helper.setArguments(arguments);
        helper.setWorkingDirectory(m_workspace->path());
#ifdef Q_OS_WIN
        helper.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
            args->flags |= 0x08000000;
        });
#endif
        if (!helper.startDetached())
        {
            fail(QStringLiteral("restartFailed"));
            return;
        }
        // The detached helper owns staging and rollback after the current process exits.
        m_workspace->setAutoRemove(false);
        setStage(QStringLiteral("restarting"), 0.98);
        emit restartReady();
    });
    setStage(QStringLiteral("extracting"), 0.9);
    m_timer.start(120000);
    m_process->start(program, arguments);
}

void ExampleUpdater::confirmReady(const QString &result)
{
    if (result.isEmpty())
        return;
    const QFileInfo fileInfo(result);
    const QFileInfo workspace(fileInfo.absolutePath());
    QFile file(result);
    if (fileInfo.fileName() != QStringLiteral("job.json") || fileInfo.isSymLink() || workspace.isSymLink()
        || !workspace.fileName().startsWith(QStringLiteral(".eui-update-"))
        || !file.open(QIODevice::ReadOnly) || file.size() > 16384)
        return;
    const auto job = QJsonDocument::fromJson(file.readAll()).object();
    if (QFileInfo(job.value(QStringLiteral("target")).toString()).absoluteFilePath() != QFileInfo(m_directory).absoluteFilePath()
        || job.value(QStringLiteral("version")).toString() != currentVersion()
        || !QFileInfo::exists(workspace.filePath() + QStringLiteral("/installed")))
        return;
    if (!writeFile(result + QStringLiteral(".ready"), currentVersion().toUtf8()))
        return;
    m_latestVersion = currentVersion();
    setStage(QStringLiteral("completed"), 1);
}
