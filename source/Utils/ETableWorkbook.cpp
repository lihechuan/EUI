#include <QFile>
#include <QBuffer>
#include <QSaveFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
#include <QtCore/private/qzipreader_p.h>
#include <QtCore/private/qzipwriter_p.h>
#else
#include <QtGui/private/qzipreader_p.h>
#include <QtGui/private/qzipwriter_p.h>
#endif

#include "ETableIO.h"

namespace
{
QString columnName(int index)
{
    QString result;
    do
    {
        result.prepend(QChar('A' + index % 26));
        index = index / 26 - 1;
    } while (index >= 0);
    return result;
}

int columnIndex(const QString &reference)
{
    int result = 0;
    for (const QChar character : reference)
    {
        if (character < 'A' || character > 'Z')
            break;
        result = result * 26 + character.unicode() - 'A' + 1;
        if (result > 16384)
            return -1;
    }
    return result - 1;
}

QVariantMap failure(const QString &message)
{
    return {{QStringLiteral("ok"), false}, {QStringLiteral("message"), message}};
}
}

QVariantMap ETableIO::writeWorkbook(const QUrl &url, const QVariantList &matrix) const
{
    if (!url.isLocalFile() || matrix.size() > 1048576)
        return failure(QStringLiteral("Specify a local XLSX file within worksheet limits"));
    QByteArray worksheet;
    QXmlStreamWriter xml(&worksheet);
    xml.writeStartDocument();
    xml.writeStartElement("worksheet");
    xml.writeDefaultNamespace("http://schemas.openxmlformats.org/spreadsheetml/2006/main");
    xml.writeStartElement("sheetData");
    for (int rowIndex = 0; rowIndex < matrix.size(); ++rowIndex)
    {
        const QVariantList row = matrix[rowIndex].toList();
        if (row.size() > 16384)
            return failure(QStringLiteral("The worksheet exceeds 16384 columns"));
        xml.writeStartElement("row");
        xml.writeAttribute("r", QString::number(rowIndex + 1));
        for (int index = 0; index < row.size(); ++index)
        {
            const QVariant value = row[index];
            xml.writeStartElement("c");
            xml.writeAttribute("r", columnName(index) + QString::number(rowIndex + 1));
            const int type = value.userType();
            if (type == QMetaType::Bool)
            {
                xml.writeAttribute("t", "b");
                xml.writeTextElement("v", value.toBool() ? "1" : "0");
            }
            else if (type == QMetaType::Int || type == QMetaType::UInt || type == QMetaType::LongLong
                     || type == QMetaType::ULongLong || type == QMetaType::Double || type == QMetaType::Float)
            {
                xml.writeTextElement("v", value.toString());
            }
            else
            {
                xml.writeAttribute("t", "inlineStr");
                xml.writeStartElement("is");
                xml.writeStartElement("t");
                xml.writeAttribute("xml:space", "preserve");
                xml.writeCharacters(value.toString());
                xml.writeEndElement();
                xml.writeEndElement();
            }
            xml.writeEndElement();
        }
        xml.writeEndElement();
        if (worksheet.size() > 64 * 1024 * 1024)
            return failure(QStringLiteral("The worksheet exceeds 64 MiB"));
    }
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndDocument();
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QZipWriter zip(&buffer);
    zip.addFile("[Content_Types].xml", QByteArrayLiteral("<?xml version=\"1.0\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\"><Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/><Default Extension=\"xml\" ContentType=\"application/xml\"/><Override PartName=\"/xl/workbook.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml\"/><Override PartName=\"/xl/worksheets/sheet1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml\"/></Types>"));
    zip.addFile("_rels/.rels", QByteArrayLiteral("<?xml version=\"1.0\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"xl/workbook.xml\"/></Relationships>"));
    zip.addFile("xl/workbook.xml", QByteArrayLiteral("<?xml version=\"1.0\"?><workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"><sheets><sheet name=\"Table\" sheetId=\"1\" r:id=\"rId1\"/></sheets></workbook>"));
    zip.addFile("xl/_rels/workbook.xml.rels", QByteArrayLiteral("<?xml version=\"1.0\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet\" Target=\"worksheets/sheet1.xml\"/></Relationships>"));
    zip.addFile("xl/worksheets/sheet1.xml", worksheet);
    zip.close();
    if (zip.status() != QZipWriter::NoError)
        return failure(QStringLiteral("XLSX output failed"));
    QSaveFile file(url.toLocalFile());
    if (!file.open(QIODevice::WriteOnly) || file.write(buffer.data()) != buffer.data().size() || !file.commit())
        return failure(file.errorString());
    return {{QStringLiteral("ok"), true}};
}

QVariantMap ETableIO::readWorkbook(const QUrl &url) const
{
    if (!url.isLocalFile())
        return failure(QStringLiteral("A local XLSX file is required"));
    QFile file(url.toLocalFile());
    if (!file.open(QIODevice::ReadOnly))
        return failure(file.errorString());
    if (file.size() > 64 * 1024 * 1024)
        return failure(QStringLiteral("The input exceeds 64 MiB"));
    if (file.peek(4) != QByteArrayLiteral("PK\x03\x04"))
        return failure(QStringLiteral("Invalid XLSX ZIP signature"));
    QZipReader zip(&file);
    const auto entries = zip.fileInfoList();
    qint64 total = 0;
    if (entries.isEmpty() || entries.size() > 2048)
        return failure(QStringLiteral("Invalid or oversized XLSX archive"));
    for (const auto &entry : entries)
    {
        if (entry.size < 0 || entry.size > 64 * 1024 * 1024 || entry.isSymLink)
            return failure(QStringLiteral("Invalid XLSX archive entry"));
        total += entry.size;
    }
    if (total > 64 * 1024 * 1024)
        return failure(QStringLiteral("The decompressed workbook exceeds 64 MiB"));
    QString relation;
    QXmlStreamReader workbook(zip.fileData("xl/workbook.xml"));
    while (!workbook.atEnd())
    {
        workbook.readNext();
        if (workbook.isStartElement() && workbook.name() == QStringLiteral("sheet"))
        {
            relation = workbook.attributes().value("http://schemas.openxmlformats.org/officeDocument/2006/relationships", "id").toString();
            break;
        }
    }
    QString sheet;
    QXmlStreamReader relations(zip.fileData("xl/_rels/workbook.xml.rels"));
    while (!relations.atEnd())
    {
        relations.readNext();
        if (relations.isStartElement() && relations.name() == QStringLiteral("Relationship")
            && relations.attributes().value("Id") == relation && relations.attributes().value("TargetMode") != QStringLiteral("External"))
        {
            sheet = relations.attributes().value("Target").toString();
            sheet = sheet.startsWith('/') ? sheet.mid(1) : "xl/" + sheet;
            break;
        }
    }
    if (sheet.isEmpty() || sheet.contains(".."))
        return failure(QStringLiteral("No local worksheet found"));
    QStringList strings;
    QXmlStreamReader shared(zip.fileData("xl/sharedStrings.xml"));
    QString string;
    while (!shared.atEnd())
    {
        shared.readNext();
        if (shared.isStartElement() && shared.name() == QStringLiteral("si"))
            string.clear();
        else if (shared.isStartElement() && shared.name() == QStringLiteral("t"))
            string += shared.readElementText();
        else if (shared.isEndElement() && shared.name() == QStringLiteral("si"))
            strings.append(string);
    }
    const QByteArray bytes = zip.fileData(sheet);
    if (bytes.isEmpty() || zip.status() != QZipReader::NoError)
        return failure(QStringLiteral("The worksheet is unavailable"));
    QXmlStreamReader xml(bytes);
    QVariantList matrix;
    QVariantList row;
    QString type;
    QString content;
    int column = -1;
    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.tokenType() == QXmlStreamReader::DTD)
            return failure(QStringLiteral("DTD content is unsupported"));
        if (xml.isStartElement() && xml.name() == QStringLiteral("row"))
        {
            row.clear();
            const int index = xml.attributes().value("r").toInt();
            if (index > 1048576 || index < 0)
                return failure(QStringLiteral("Invalid worksheet row"));
            while (matrix.size() < index - 1)
                matrix.append(QVariant(QVariantList()));
        }
        else if (xml.isStartElement() && xml.name() == QStringLiteral("c"))
        {
            column = xml.attributes().hasAttribute("r") ? columnIndex(xml.attributes().value("r").toString()) : row.size();
            if (column < 0 || column >= 16384)
                return failure(QStringLiteral("Invalid worksheet column"));
            type = xml.attributes().value("t").toString();
            content.clear();
        }
        else if (xml.isStartElement() && (xml.name() == QStringLiteral("v") || xml.name() == QStringLiteral("t")))
            content += xml.readElementText();
        else if (xml.isEndElement() && xml.name() == QStringLiteral("c"))
        {
            QVariant value = content;
            if (type == "s")
            {
                bool valid = false;
                const int index = content.toInt(&valid);
                if (!valid || index < 0 || index >= strings.size())
                    return failure(QStringLiteral("Invalid shared string index"));
                value = strings[index];
            }
            else if (type == "b")
                value = content == "1";
            else if (type.isEmpty() || type == "n")
            {
                bool valid = false;
                const double number = content.toDouble(&valid);
                value = valid ? QVariant(number) : QVariant(content);
            }
            while (row.size() <= column)
                row.append(QVariant());
            row[column] = value;
        }
        else if (xml.isEndElement() && xml.name() == QStringLiteral("row"))
            matrix.append(QVariant(row));
        if (matrix.size() > 1048576)
            return failure(QStringLiteral("The worksheet exceeds row limits"));
    }
    if (xml.hasError())
        return failure(xml.errorString());
    return {{QStringLiteral("ok"), true}, {QStringLiteral("matrix"), matrix}};
}
