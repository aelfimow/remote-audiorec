#include <Windows.h>

#include "WndProcHandler.h"
#include "WMSizeHandler.h"
#include "main.h"
#include "Console.h"


WM_SIZE_Handler::WM_SIZE_Handler() :
    WndProcHandler {}
{
}

WM_SIZE_Handler::~WM_SIZE_Handler()
{
}

LRESULT WM_SIZE_Handler::operator()([[maybe_unused]] HWND hwnd, [[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
    if (console != nullptr)
    {
        console->MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
    }

    return 0;
}
