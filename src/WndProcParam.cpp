#include <Windows.h>

#include "WndProcParam.h"


WndProcParam::WndProcParam(HWND hwnd, WPARAM wParam, LPARAM lParam) :
    m_hwnd { hwnd },
    m_wParam { wParam },
    m_lParam { lParam }
{
}

WndProcParam::~WndProcParam()
{
}

HWND WndProcParam::hwnd() const
{
    return m_hwnd;
}

WPARAM WndProcParam::wParam() const
{
    return m_wParam;
}

LPARAM WndProcParam::lParam() const
{
    return m_lParam;
}
