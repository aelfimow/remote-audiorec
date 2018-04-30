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
#include "Console.h"


#define MY_PORT         "50000"


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

    *console << TEXT("Thread started") << Console::eol;

    while (true)
    {
        *console << TEXT("Waiting for commands") << Console::eol;

        WSAStarter wsa;

        if (wsa.is_error())
        {
            *console << TEXT("WSAStarter failed with error") << Console::eol;
            break;
        }

        AddrInfo ai { MY_PORT };
        if (ai.is_error())
        {
            *console << TEXT("AddrInfo failed") << Console::eol;
            break;
        }

        SocketListener sockListener { ai };

        if (sockListener.is_error())
        {
            *console << TEXT("Socket listener creation failed") << Console::eol;
            break;
        }

        sockListener.listen();

        if (sockListener.is_error())
        {
            *console << TEXT("Socket listening failed") << Console::eol;
            break;
        }

        SocketScope clientSock { sockListener.accept() };

        sockListener.close();

        if (clientSock.is_error())
        {
            *console << TEXT("SocketScope creation failed") << Console::eol;
            break;
        }

        while (true)
        {
            const std::string cmd = clientSock.recv();

            if (clientSock.is_error())
            {
                break;
            }

            if (0 == cmd.length())
            {
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

        if (clientSock.is_error())
        {
            *console << TEXT("SocketScope recv failed") << Console::eol;
            break;
        }
    }

    *console << TEXT("Thread exit") << Console::eol;

    ExitThread(0);

    return 0;
}
