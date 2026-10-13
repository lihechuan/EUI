#include <QRandomGenerator>

#include "RandomUtils.h"

namespace
{
constexpr char NANOID_ALPHABET[]   = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
constexpr int NANOID_ALPHABET_SIZE = sizeof(NANOID_ALPHABET) - 1;
}

QString RandomUtils::nanoid(int size)
{
    size = qMax(0, size);
    QString id;
    id.reserve(size);

    for (int i = 0; i < size; ++i)
    {
        int r = QRandomGenerator::global()->bounded(NANOID_ALPHABET_SIZE);
        id.append(NANOID_ALPHABET[r]);
    }
    return id;
}

RandomUtils::RandomUtils(QObject *parent) : QObject(parent)
{
}
