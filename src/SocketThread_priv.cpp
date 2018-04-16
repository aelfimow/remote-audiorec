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

    while (1)
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

        auto ClientSocket = sockListener.accept();

        if (ClientSocket == INVALID_SOCKET)
        {
            EditPrintf(pInst->hwndEdit, TEXT("accept failed with error: %d\n"), WSAGetLastError());
            break;
        }

        sockListener.close();

        std::vector<char> recvbuf(1024);

        auto recv_bytes = recv(ClientSocket, recvbuf.data(), recvbuf.size(), 0);

        if (recv_bytes > 0)
        {
            std::string::size_type total_chars = recv_bytes;

            std::string str { recvbuf.data(), total_chars };

            EditPrintf(pInst->hwndEdit, TEXT("Bytes received %d, message: %s\n"), recv_bytes, str.c_str());

            if (str == "start")
            {
                (void)PostMessage(pInst->hwnd, WM_USER_START, 0, 0);
            }

            if (str == "stop")
            {
                (void)PostMessage(pInst->hwnd, WM_USER_STOP, 0, 0);
            }
        }

        if (recv_bytes < 0)
        {
            EditPrintf(pInst->hwndEdit, TEXT("recv failed with error: %d\n"), WSAGetLastError());
            closesocket(ClientSocket);
            break;
        }

        auto iResult = shutdown(ClientSocket, SD_SEND);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(pInst->hwndEdit, TEXT("shutdown failed with error: %d\n"), WSAGetLastError());
            closesocket(ClientSocket);
            break;
        }

        closesocket(ClientSocket);
    }

    EditPrintf(pInst->hwndEdit, TEXT("Thread exit"));

    ExitThread(0);

    return 0;
}
