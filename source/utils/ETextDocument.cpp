#include <QUrl>
#include <QColor>
#include <QTextList>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextListFormat>

#include "ETextDocument.h"

ETextDocument::ETextDocument(QObject *parent) : QObject(parent)
{
}

QQuickTextDocument *ETextDocument::document() const
{
    return document_;
}

void ETextDocument::setDocument(QQuickTextDocument *document)
{
    if (document_ == document)
        return;
    if (document_)
        disconnect(document_, nullptr, this, nullptr);
    document_ = document;
    if (document_)
        connect(document_, &QObject::destroyed, this, [this]() { document_ = nullptr; emit documentChanged(); });
    emit documentChanged();
}

QVariantMap ETextDocument::formatAt(int position) const
{
    if (!document_ || !document_->textDocument())
        return {};
    QTextCursor cursor(document_->textDocument());
    cursor.setPosition(qBound(0, position, document_->textDocument()->characterCount() - 1));
    const auto format = cursor.charFormat();
    return {{QStringLiteral("bold"), format.fontWeight() >= QFont::Bold},
            {QStringLiteral("italic"), format.fontItalic()},
            {QStringLiteral("underline"), format.fontUnderline()},
            {QStringLiteral("strike"), format.fontStrikeOut()},
            {QStringLiteral("color"), format.foreground().color()},
            {QStringLiteral("size"), format.fontPointSize()},
            {QStringLiteral("alignment"), int(cursor.blockFormat().alignment())}};
}

bool ETextDocument::apply(int start, int end, const QString &operation, const QVariant &value)
{
    auto *document = document_ ? document_->textDocument() : nullptr;
    if (!document || start < 0 || end < 0 || start >= document->characterCount() || end >= document->characterCount())
        return false;
    QTextCursor cursor(document);
    cursor.setPosition(qMin(start, end));
    cursor.setPosition(qMax(start, end), QTextCursor::KeepAnchor);
    QTextCharFormat format;
    cursor.beginEditBlock();
    if (operation == QStringLiteral("bold"))
        format.setFontWeight(value.toBool() ? QFont::Bold : QFont::Normal);
    else if (operation == QStringLiteral("italic"))
        format.setFontItalic(value.toBool());
    else if (operation == QStringLiteral("underline"))
        format.setFontUnderline(value.toBool());
    else if (operation == QStringLiteral("strike"))
        format.setFontStrikeOut(value.toBool());
    else if (operation == QStringLiteral("color") && QColor(value.toString()).isValid())
        format.setForeground(QColor(value.toString()));
    else if (operation == QStringLiteral("size") && value.toDouble() >= 6 && value.toDouble() <= 144)
        format.setFontPointSize(value.toDouble());
    else if (operation == QStringLiteral("family"))
        format.setFontFamily(value.toString());
    else if (operation == QStringLiteral("align"))
    {
        QTextBlockFormat block;
        const int alignment = value.toInt();
        if (alignment != Qt::AlignLeft && alignment != Qt::AlignRight && alignment != Qt::AlignHCenter && alignment != Qt::AlignJustify)
        {
            cursor.endEditBlock();
            return false;
        }
        block.setAlignment(Qt::Alignment(alignment));
        cursor.mergeBlockFormat(block);
        cursor.endEditBlock();
        return true;
    }
    else if (operation == QStringLiteral("list"))
    {
        QTextListFormat list;
        list.setStyle(value.toString() == QStringLiteral("ordered") ? QTextListFormat::ListDecimal : QTextListFormat::ListDisc);
        cursor.createList(list);
        cursor.endEditBlock();
        return true;
    }
    else if (operation == QStringLiteral("link"))
    {
        const QUrl url(value.toString());
        if (url.scheme() != QStringLiteral("http") && url.scheme() != QStringLiteral("https") && url.scheme() != QStringLiteral("mailto"))
        {
            cursor.endEditBlock();
            return false;
        }
        format.setAnchor(true);
        format.setAnchorHref(url.toString());
        format.setFontUnderline(true);
    }
    else if (operation == QStringLiteral("clear"))
    {
        cursor.setCharFormat(QTextCharFormat());
        cursor.endEditBlock();
        return true;
    }
    else
    {
        cursor.endEditBlock();
        return false;
    }
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    cursor.endEditBlock();
    return true;
}
