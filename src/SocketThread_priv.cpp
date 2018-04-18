#include <winsock2.h>
#include <Windows.h>
#include <process.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>

#include "main.h"
#include "SocketThread_param.h"
#include "SocketThread_priv.h"
#include "WSAStarter.h"
#include "AddrInfo.h"
#include "SocketListener.h"
#include "SocketScope.h"


#define MY_PORT         "50000"


extern void EditPrintf(HWND hwndEdit, const TCHAR *szFormat, ...);


SocketThread_priv::SocketThread_priv(const struct ThreadParams &params) :
    hThread { NULL },
    hEvent { params.event },
    hwnd { params.hwnd },
    hwndEdit { params.hwndEdit },
    threadID { 0 }
{
}

SocketThread_priv::~SocketThread_priv()
{
}

void SocketThread_priv::start()
{
    hThread = CreateThread(NULL, 0, &threadFunc, (LPVOID)this, 0, &threadID);

    if (NULL != hThread)
    {
        WaitForSingleObject(hEvent, INFINITE);
    }
}

void SocketThread_priv::stop()
{
}

DWORD WINAPI SocketThread_priv::threadFunc(LPVOID pvoid)
{
    SocketThread_priv * const pInst = reinterpret_cast<SocketThread_priv*>(pvoid);

    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    SetEvent(pInst->hEvent);

    EditPrintf(pInst->hwndEdit, TEXT("Thread started"));

    while (true)
    {
        EditPrintf(pInst->hwndEdit, TEXT("Waiting for commands"));

        WSAStarter wsa;

        if (wsa.is_error())
        {
            EditPrintf(pInst->hwndEdit, TEXT("WSAStarter failed with error\n"));
            break;
        }

        AddrInfo ai { MY_PORT };
        if (ai.is_error())
        {
            EditPrintf(pInst->hwndEdit, TEXT("AddrInfo failed"));
            break;
        }

        SocketListener sockListener { ai };

        if (sockListener.is_error())
        {
            EditPrintf(pInst->hwndEdit, TEXT("Socket listener creation failed"));
            break;
        }

        sockListener.listen();

        if (sockListener.is_error())
        {
            EditPrintf(pInst->hwndEdit, TEXT("Socket listening failed"));
            break;
        }

        SocketScope clientSock { sockListener.accept() };

        sockListener.close();

        if (clientSock.is_error())
        {
            EditPrintf(pInst->hwndEdit, TEXT("SocketScope creation failed"));
            break;
        }

        const std::string cmd = clientSock.recv();

        if (clientSock.is_error())
        {
            EditPrintf(pInst->hwndEdit, TEXT("SocketScope recv failed"));
            break;
        }

        if (cmd == "start")
        {
            (void)PostMessage(pInst->hwnd, WM_USER_START, 0, 0);
        }

        if (cmd == "stop")
        {
            (void)PostMessage(pInst->hwnd, WM_USER_STOP, 0, 0);
        }
    }

    EditPrintf(pInst->hwndEdit, TEXT("Thread exit"));

    ExitThread(0);

    return 0;
}
