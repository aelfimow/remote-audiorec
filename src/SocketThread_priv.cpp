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

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        int iResult = getaddrinfo(NULL, MY_PORT, &hints, &result);

        if (iResult != 0)
        {
            EditPrintf(pInst->hwndEdit, TEXT("getaddrinfo failed with error: %d\n"), iResult);
            break;
        }

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET)
        {
            EditPrintf(pInst->hwndEdit, TEXT("socket failed with error: %ld\n"), WSAGetLastError());
            freeaddrinfo(result);
            break;
        }

        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(pInst->hwndEdit, TEXT("bind failed with error: %d\n"), WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            break;
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(pInst->hwndEdit, TEXT("listen failed with error: %d\n"), WSAGetLastError());
            closesocket(ListenSocket);
            break;
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);

        if (ClientSocket == INVALID_SOCKET)
        {
            EditPrintf(pInst->hwndEdit, TEXT("accept failed with error: %d\n"), WSAGetLastError());
            closesocket(ListenSocket);
            break;
        }

        closesocket(ListenSocket);

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

        iResult = shutdown(ClientSocket, SD_SEND);

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
