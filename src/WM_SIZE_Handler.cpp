#include <Windows.h>

#include "WndProcParam.h"
#include "WndProcHandler.h"
#include "WM_SIZE_Handler.h"
#include "main.h"
#include "Console.h"


WM_SIZE_Handler::WM_SIZE_Handler() :
    WndProcHandler {}
{
}

WM_SIZE_Handler::~WM_SIZE_Handler()
{
}

LRESULT WM_SIZE_Handler::operator()(const WndProcParam &param)
{
    if (console != nullptr)
    {
        console->MoveWindow(0, 0, LOWORD(param.lParam()), HIWORD(param.lParam()));
    }

    return 0;
}
