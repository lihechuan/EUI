#pragma once

#include <utility>

// 单例只在第一次调用时创建，后续调用忽略传入参数。
template <typename T> class Singleton
{
public:
    static T *getInstance()
    {
        static T *instance = new T();
        return instance;
    }

    template <typename... Args> static T *getInstance(Args &&...args)
    {
        static T *instance = new T(std::forward<Args>(args)...);
        return instance;
    }
};

#define SINGLETON(Class)                                                                                               \
private:                                                                                                               \
    friend class Singleton<Class>;                                                                                     \
                                                                                                                       \
public:                                                                                                                \
    static Class *getInstance()                                                                                        \
    {                                                                                                                  \
        return Singleton<Class>::getInstance();                                                                        \
    }                                                                                                                  \
    template <typename... Args> static Class *getInstance(Args &&...args)                                              \
    {                                                                                                                  \
        return Singleton<Class>::getInstance(std::forward<Args>(args)...);                                             \
    }
