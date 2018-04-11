#include <winsock2.h>
#include <Windows.h>
#include <process.h>
#include <ws2tcpip.h>

#include "main.h"
#include "SocketThread_param.h"
#include "SocketThread_priv.h"


#define MY_PORT         "50000"
#define RX_BUFFER_SIZE  1024


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

        WSADATA wsaData;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        char recvbuf[RX_BUFFER_SIZE];

        int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

        if (iResult != 0)
        {
            EditPrintf(pInst->hwndEdit, TEXT("WSAStartup failed with error: %d\n"), iResult);
            break;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        iResult = getaddrinfo(NULL, MY_PORT, &hints, &result);

        if (iResult != 0)
        {
            EditPrintf(pInst->hwndEdit, TEXT("getaddrinfo failed with error: %d\n"), iResult);
            WSACleanup();
            break;
        }

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET)
        {
            EditPrintf(pInst->hwndEdit, TEXT("socket failed with error: %ld\n"), WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            break;
        }

        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(pInst->hwndEdit, TEXT("bind failed with error: %d\n"), WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            break;
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(pInst->hwndEdit, TEXT("listen failed with error: %d\n"), WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            break;
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);

        if (ClientSocket == INVALID_SOCKET)
        {
            EditPrintf(pInst->hwndEdit, TEXT("accept failed with error: %d\n"), WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            break;
        }

        closesocket(ListenSocket);

        do
        {
            ZeroMemory(recvbuf, sizeof(recvbuf));
            iResult = recv(ClientSocket, recvbuf, sizeof(recvbuf) - 1, 0);

            if (iResult > 0)
            {
                EditPrintf(pInst->hwndEdit, TEXT("Bytes received %d, message: %s\n"), iResult, recvbuf);

                if (0 == strcmp(recvbuf, "start"))
                {
                    (void)PostMessage(pInst->hwnd, WM_USER_START, 0, 0);
                }

                if (0 == strcmp(recvbuf, "stop"))
                {
                    (void)PostMessage(pInst->hwnd, WM_USER_STOP, 0, 0);
                }
            }

            if (iResult < 0)
            {
                EditPrintf(pInst->hwndEdit, TEXT("recv failed with error: %d\n"), WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                break;
            }

        } while (iResult > 0);

        iResult = shutdown(ClientSocket, SD_SEND);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(pInst->hwndEdit, TEXT("shutdown failed with error: %d\n"), WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            break;
        }

        closesocket(ClientSocket);
        WSACleanup();
    }

    EditPrintf(pInst->hwndEdit, TEXT("Thread exit"));

    ExitThread(0);

    return 0;
}
