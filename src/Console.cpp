#include <Windows.h>
#include <vector>

#include "Console.h"
#include "main.h"

const TCHAR Console::eol[] = TEXT("\r\n");

Console::Console(HWND hwnd, HINSTANCE hInstance) :
    m_hwnd { CreateWindow(
            TEXT("edit"),
            NULL,
            (WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | \
             ES_READONLY | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL),
            0,
            0,
            0,
            0,
            hwnd,
            (HMENU)ID_EDIT,
            hInstance,
            NULL) },
    m_error { false }
{
    m_error = (m_hwnd == nullptr);

    if (!m_error)
    {
        // Set limit of text to max.
        SendMessage(m_hwnd, EM_LIMITTEXT, 0, 0L);
    }
}

Console::~Console()
{
}

bool Console::is_error() const
{
    return m_error;
}

void Console::SetFocus()
{
    ::SetFocus(m_hwnd);
}

void Console::MoveWindow(size_t x0, size_t y0, size_t x1, size_t y1)
{
    ::MoveWindow(m_hwnd, x0, y0, x1, y1, TRUE);
}

Console& Console::operator<<(const TCHAR *str)
{
    SendMessage(m_hwnd, EM_SETSEL, (WPARAM)-1, (WPARAM)-1);
    SendMessage(m_hwnd, EM_REPLACESEL, FALSE, (LPARAM)str);
    SendMessage(m_hwnd, EM_SCROLLCARET, 0, 0);

    return *this;
}

Console& Console::operator<<(size_t value)
{
    std::vector<TCHAR> buffer(1024);

    wsprintf(buffer.data(), "%u", value);

    SendMessage(m_hwnd, EM_SETSEL, (WPARAM)-1, (WPARAM)-1);
    SendMessage(m_hwnd, EM_REPLACESEL, FALSE, (LPARAM)buffer.data());
    SendMessage(m_hwnd, EM_SCROLLCARET, 0, 0);

    return *this;
}
