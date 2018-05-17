#include <Windows.h>

#include "WndProcHandler.h"
#include "WM_SET_FOCUS_Handler.h"
#include "main.h"
#include "Console.h"


WM_SETFOCUS_Handler::WM_SETFOCUS_Handler() :
    WndProcHandler {}
{
}

WM_SETFOCUS_Handler::~WM_SETFOCUS_Handler()
{
}

LRESULT WM_SETFOCUS_Handler::operator()([[maybe_unused]] HWND hwnd, [[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
    if (console != nullptr)
    {
        console->SetFocus();
    }

    return 0;
}
