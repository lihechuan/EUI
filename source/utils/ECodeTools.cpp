#include <QSet>
#include <QHash>
#include <QColor>
#include <QClipboard>
#include <QGuiApplication>

#include "ECodeTools.h"

ECodeTools::ECodeTools(QObject *parent) : QObject(parent)
{
}

bool ECodeTools::copyText(const QString &text) const
{
    auto *clipboard = QGuiApplication::clipboard();
    if (!clipboard)
        return false;
    clipboard->setText(text);
    return true;
}

QString ECodeTools::highlight(const QString &code, const QVariantMap &colors) const
{
    return highlightCode(code, colors);
}

QString ECodeTools::highlightCode(const QString &code, const QVariantMap &colors)
{
    static const QSet<QString> literals = {
        QStringLiteral("null"), QStringLiteral("true"), QStringLiteral("false"), QStringLiteral("undefined")};
    static const QSet<QString> keywords = {
        QStringLiteral("as"),      QStringLiteral("if"),       QStringLiteral("on"),       QStringLiteral("for"),
        QStringLiteral("let"),     QStringLiteral("new"),      QStringLiteral("var"),      QStringLiteral("else"),
        QStringLiteral("case"),    QStringLiteral("break"),    QStringLiteral("const"),    QStringLiteral("while"),
        QStringLiteral("import"),  QStringLiteral("return"),   QStringLiteral("signal"),   QStringLiteral("switch"),
        QStringLiteral("default"), QStringLiteral("property"), QStringLiteral("readonly"), QStringLiteral("continue"),
        QStringLiteral("function")};
    QHash<QString, QString> palette;
    for (auto iterator = colors.constBegin(); iterator != colors.constEnd(); ++iterator)
    {
        const QColor color(iterator.value().toString());
        if (color.isValid())
            palette.insert(iterator.key(), color.name());
    }
    const auto preserveWhitespace = [](const QString &value)
    {
        QString result = value.toHtmlEscaped();
        result.replace(QStringLiteral("\r\n"), QStringLiteral("\n"));
        result.replace(QLatin1Char('\r'), QLatin1Char('\n'));
        result.replace(QLatin1Char('\t'), QStringLiteral("&nbsp;&nbsp;&nbsp;&nbsp;"));
        result.replace(QLatin1Char(' '), QStringLiteral("&nbsp;"));
        result.replace(QLatin1Char('\n'), QStringLiteral("<br/>"));
        return result;
    };
    const auto span = [&palette](const QString &value, const QString &role)
    {
        const QString color = palette.value(role);
        return color.isEmpty() ? value : QStringLiteral("<span style=\"color:%1;\">%2</span>").arg(color, value);
    };
    const auto identifierStart = [](QChar character) { return character.isLetter() || character == QLatin1Char('_'); };
    const auto identifierPart  = [&identifierStart](QChar character)
    { return identifierStart(character) || character.isDigit(); };
    QString result;
    result.reserve(code.size() * 2);
    const QString punctuation = QStringLiteral("[]{}():,.");
    int position              = 0;
    while (position < code.size())
    {
        const QChar character     = code.at(position);
        const QChar nextCharacter = position + 1 < code.size() ? code.at(position + 1) : QChar();
        if (character.isSpace())
        {
            int end = position + 1;
            while (end < code.size() && code.at(end).isSpace())
                ++end;
            result += preserveWhitespace(code.mid(position, end - position));
            position = end;
            continue;
        }
        if (character == QLatin1Char('/') && (nextCharacter == QLatin1Char('/') || nextCharacter == QLatin1Char('*')))
        {
            const bool block = nextCharacter == QLatin1Char('*');
            int end          = block ? code.indexOf(QStringLiteral("*/"), position + 2)
                                     : code.indexOf(QLatin1Char('\n'), position + 2);
            end              = end < 0 ? code.size() : end + (block ? 2 : 0);
            result += span(preserveWhitespace(code.mid(position, end - position)), QStringLiteral("comment"));
            position = end;
            continue;
        }
        if (character == QLatin1Char('"') || character == QLatin1Char('\'') || character == QLatin1Char('`'))
        {
            int end = position + 1;
            while (end < code.size())
            {
                if (code.at(end) == QLatin1Char('\\'))
                {
                    end = qMin(end + 2, code.size());
                    continue;
                }
                if (code.at(end++) == character)
                    break;
            }
            result += span(preserveWhitespace(code.mid(position, end - position)), QStringLiteral("string"));
            position = end;
            continue;
        }
        if (character.isDigit())
        {
            int end                = position + 1;
            const bool hexadecimal = character == QLatin1Char('0') &&
                                     (nextCharacter == QLatin1Char('x') || nextCharacter == QLatin1Char('X'));
            if (hexadecimal)
                ++end;
            while (end < code.size())
            {
                const QChar digit = code.at(end);
                if (digit.isDigit() || (!hexadecimal && digit == QLatin1Char('.')) ||
                    (hexadecimal && ((digit >= QLatin1Char('a') && digit <= QLatin1Char('f')) ||
                                     (digit >= QLatin1Char('A') && digit <= QLatin1Char('F')))))
                    ++end;
                else
                    break;
            }
            if (!hexadecimal && end < code.size() &&
                (code.at(end) == QLatin1Char('e') || code.at(end) == QLatin1Char('E')))
            {
                int exponent = end + 1;
                if (exponent < code.size() &&
                    (code.at(exponent) == QLatin1Char('+') || code.at(exponent) == QLatin1Char('-')))
                    ++exponent;
                const int digits = exponent;
                while (exponent < code.size() && code.at(exponent).isDigit())
                    ++exponent;
                if (exponent > digits)
                    end = exponent;
            }
            result += span(code.mid(position, end - position).toHtmlEscaped(), QStringLiteral("number"));
            position = end;
            continue;
        }
        if (identifierStart(character))
        {
            int end = position + 1;
            while (end < code.size() && identifierPart(code.at(end)))
                ++end;
            const QString token = code.mid(position, end - position);
            int next            = end;
            int previous        = position - 1;
            while (next < code.size() && code.at(next).isSpace())
                ++next;
            while (previous >= 0 && code.at(previous).isSpace())
                --previous;
            QString role;
            if (keywords.contains(token))
                role = QStringLiteral("keyword");
            else if (literals.contains(token))
                role = QStringLiteral("literal");
            else if (next < code.size() && code.at(next) == QLatin1Char(':'))
                role = QStringLiteral("property");
            else if (previous >= 0 && code.at(previous) == QLatin1Char('.'))
                role = QStringLiteral("member");
            else if (character.isUpper())
                role = QStringLiteral("type");
            result += span(token.toHtmlEscaped(), role);
            position = end;
            continue;
        }
        const QString escaped = QString(character).toHtmlEscaped();
        result += punctuation.contains(character) ? span(escaped, QStringLiteral("punctuation")) : escaped;
        ++position;
    }
    return span(result, QStringLiteral("plain"));
}
