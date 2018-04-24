#include <Windows.h>

#include "WndProcHandler.h"
#include "WMSetFocusHandler.h"
#include "main.h"
#include "Console.h"


WMSetFocusHandler::WMSetFocusHandler() :
    WndProcHandler {}
{
}

WMSetFocusHandler::~WMSetFocusHandler()
{
}

LRESULT WMSetFocusHandler::operator()(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    hwnd = hwnd;
    wParam = wParam;
    lParam = lParam;

    if (console != nullptr)
    {
        console->SetFocus();
    }

    return 0;
}
