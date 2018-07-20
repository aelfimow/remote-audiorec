#include <Windows.h>

#include "WndProcParam.h"
#include "WndProcHandler.h"
#include "WM_CREATE_Handler.h"
#include "main.h"
#include "SocketThread_param.h"
#include "SocketThread.h"
#include "Console.h"


WM_CREATE_Handler::WM_CREATE_Handler() :
    WndProcHandler {}
{
}

WM_CREATE_Handler::~WM_CREATE_Handler()
{
}

LRESULT WM_CREATE_Handler::operator()(const WndProcParam &param)
{
    console = new Console(param.hwnd(), ((LPCREATESTRUCT)param.lParam())->hInstance);

    if (console->is_error())
    {
        (void)MessageBox(NULL, TEXT("Error creating console"), TEXT("WndProc"), MB_ICONERROR);
    }

    struct ThreadParams params;
    params.hwnd = param.hwnd();
    params.hwndEdit = nullptr;
    params.event = CreateEvent(NULL, FALSE, FALSE, NULL);

    G.pSocketThread = new SocketThread { params };
    G.pSocketThread->start();

    return 0;
}
