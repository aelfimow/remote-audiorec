#include <Windows.h>

#include "WndProcHandler.h"
#include "WMCreateHandler.h"
#include "main.h"
#include "SocketThread_param.h"
#include "SocketThread.h"
#include "Console.h"


WMCreateHandler::WMCreateHandler() :
    WndProcHandler {}
{
}

WMCreateHandler::~WMCreateHandler()
{
}

LRESULT WMCreateHandler::operator()(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    wParam = wParam;

    console = new Console(hwnd, ((LPCREATESTRUCT)lParam)->hInstance);

    if (console->is_error())
    {
        (void)MessageBox(NULL, TEXT("Error creating console"), TEXT("WndProc"), MB_ICONERROR);
    }

    struct ThreadParams params;
    params.hwnd = hwnd;
    params.hwndEdit = nullptr;
    params.event = CreateEvent(NULL, FALSE, FALSE, NULL);

    G.pSocketThread = new SocketThread { params };
    G.pSocketThread->start();

    return 0;
}
