#include <Windows.h>

#include "WndProcParam.h"
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

LRESULT WM_SETFOCUS_Handler::operator()([[maybe_unused]] const WndProcParam &param)
{
    if (console != nullptr)
    {
        console->SetFocus();
    }

    return 0;
}
