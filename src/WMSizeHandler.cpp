#include <Windows.h>

#include "WndProcHandler.h"
#include "WMSizeHandler.h"
#include "main.h"
#include "Console.h"


WMSizeHandler::WMSizeHandler() :
    WndProcHandler {}
{
}

WMSizeHandler::~WMSizeHandler()
{
}

LRESULT WMSizeHandler::operator()(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    hwnd = hwnd;
    wParam = wParam;

    if (console != nullptr)
    {
        console->MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
    }

    return 0;
}
