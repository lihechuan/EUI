#include <QWindow>
#include <cstring>
#include <QClipboard>
#include <QGuiApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "ExampleClipboard.h"

namespace
{
constexpr int RetryLimit    = 20;
constexpr int RetryInterval = 25;
}

ExampleClipboard::ExampleClipboard(QObject *parent) : QObject(parent)
{
    m_retryTimer.setInterval(RetryInterval);
    connect(&m_retryTimer,
            &QTimer::timeout,
            this,
            [this]()
            {
                if (trySetText() || m_attempts >= RetryLimit)
                    m_retryTimer.stop();
            });
}

void ExampleClipboard::setText(const QString &text)
{
    m_text     = text;
    m_attempts = 0;
    m_retryTimer.stop();
    if (!trySetText())
        m_retryTimer.start();
}

bool ExampleClipboard::trySetText()
{
    ++m_attempts;
#ifdef Q_OS_WIN
    QWindow *window = QGuiApplication::focusWindow();
    if (!window && !QGuiApplication::topLevelWindows().isEmpty())
        window = QGuiApplication::topLevelWindows().constFirst();
    if (!window)
        return false;

    const auto owner = reinterpret_cast<HWND>(window->winId());
    if (!OpenClipboard(owner))
        return false;

    bool success   = false;
    HGLOBAL memory = nullptr;
    if (EmptyClipboard())
    {
        const auto byteCount = static_cast<SIZE_T>(m_text.size() + 1) * sizeof(wchar_t);
        memory               = GlobalAlloc(GMEM_MOVEABLE, byteCount);
        if (memory)
        {
            if (void *destination = GlobalLock(memory))
            {
                std::memcpy(destination, m_text.utf16(), byteCount);
                GlobalUnlock(memory);
                if (SetClipboardData(CF_UNICODETEXT, memory))
                {
                    memory  = nullptr;
                    success = true;
                }
            }
        }
    }
    if (memory)
        GlobalFree(memory);
    CloseClipboard();
    return success;
#else
    QGuiApplication::clipboard()->setText(m_text);
    return true;
#endif
}
