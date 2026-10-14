#pragma once

#include <QList>

namespace ArrayUtils
{
template <typename T> inline QList<T> fill(int size, const T &value)
{
    QList<T> list;

    if (size > 0)
    {
        list.reserve(size);

        for (int cIndex = 0; cIndex < size; cIndex++)
        {
            list.append(value);
        }
    }

    return list;
}
}
