#include <QFile>
#include <QtTest>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTemporaryDir>

#include "utils/EImageIO.h"

class ImageIOTest : public QObject
{
    Q_OBJECT

private slots:
    void dataAndLocal();
    void httpAndFailure();
    void invalidDestinations();
};

void ImageIOTest::dataAndLocal()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    EImageIO io;
    QSignalSpy saved(&io, &EImageIO::saved);
    const QUrl first = QUrl::fromLocalFile(directory.filePath("image.svg"));
    const QUrl second = QUrl::fromLocalFile(directory.filePath("copy.svg"));
    const QByteArray bytes("<svg xmlns=\"http://www.w3.org/2000/svg\"/>");
    QVERIFY(io.save(QUrl("data:image/svg+xml;base64," + QString::fromLatin1(bytes.toBase64())), first));
    QFile file(first.toLocalFile());
    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), bytes);
    file.close();
    QVERIFY(io.save(first, second));
    QFile copy(second.toLocalFile());
    QVERIFY(copy.open(QIODevice::ReadOnly));
    QCOMPARE(copy.readAll(), bytes);
    QCOMPARE(saved.count(), 2);
    QVERIFY(!io.busy());
}

void ImageIOTest::invalidDestinations()
{
    QTemporaryDir directory;
    EImageIO io;
    QSignalSpy failed(&io, &EImageIO::failed);
    QVERIFY(!io.save(QUrl("data:text/plain,hello"), QUrl("https://example.com/image.svg")));
    QVERIFY(!io.save(QUrl("data:text/plain,hello"), QUrl("relative.svg")));
    QVERIFY(!io.save(QUrl("ftp://example.com/image.svg"), QUrl::fromLocalFile(directory.filePath("image.svg"))));
    QVERIFY(!io.save(QUrl::fromLocalFile(directory.filePath("missing.svg")), QUrl::fromLocalFile(directory.filePath("image.svg"))));
    QVERIFY(!io.save(QUrl("data:text/plain,hello"), QUrl::fromLocalFile(directory.filePath("missing/image.svg"))));
    QCOMPARE(failed.count(), 5);
    QVERIFY(!QFile::exists(directory.filePath("image.svg")));
}

void ImageIOTest::httpAndFailure()
{
    QTemporaryDir directory;
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));
    bool fail = false;
    connect(&server, &QTcpServer::newConnection, &server, [&]() {
        auto *socket = server.nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, socket, [socket, &fail]() {
            socket->readAll();
            socket->write(fail ? "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n"
                               : "HTTP/1.1 200 OK\r\nContent-Length: 5\r\nConnection: close\r\n\r\nimage");
            socket->disconnectFromHost();
        });
        connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
    });
    EImageIO io;
    QSignalSpy saved(&io, &EImageIO::saved);
    QSignalSpy failed(&io, &EImageIO::failed);
    const QUrl source(QString("http://127.0.0.1:%1/image").arg(server.serverPort()));
    const QUrl destination = QUrl::fromLocalFile(directory.filePath("image.svg"));
    QVERIFY(io.save(source, destination));
    QVERIFY(io.busy());
    QVERIFY(!io.save(source, destination));
    QTRY_COMPARE(saved.count(), 1);
    QVERIFY(!io.busy());
    QFile file(destination.toLocalFile());
    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), QByteArray("image"));
    file.close();
    fail = true;
    QVERIFY(io.save(source, destination));
    QTRY_COMPARE(failed.count(), 2);
    QVERIFY(!io.busy());
    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), QByteArray("image"));
}

QTEST_GUILESS_MAIN(ImageIOTest)
#include "tst_image_io.moc"
