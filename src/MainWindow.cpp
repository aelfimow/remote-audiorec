#include <Windows.h>
#include <vector>
#include <map>

#include "main.h"
#include "MainWindow.h"
#include "WndProcHandler.h"
#include "WM_SIZE_Handler.h"
#include "WM_SET_FOCUS_Handler.h"
#include "WM_CREATE_Handler.h"
#include "WM_USER_START_Handler.h"
#include "WM_USER_STOP_Handler.h"
#include "MM_WIM_OPEN_Handler.h"
#include "MM_WIM_DATA_Handler.h"
#include "MM_WIM_CLOSE_Handler.h"
#include "WM_COMMAND_Handler.h"
#include "WM_DESTROY_Handler.h"


MainWindow *MainWindow::Inst = nullptr;

const TCHAR MainWindow::MainWindowName[] = TEXT("remote-audiorec");


MainWindow::MainWindow(HINSTANCE hInstance, int iCmdShow) :
    m_hInstance { hInstance },
    m_CmdShow { iCmdShow },
    m_WndProcMap { },
    m_Wndclass { },
    m_hWindow { nullptr }
{
}

MainWindow::~MainWindow()
{
    for (auto &elem: m_WndProcMap)
    {
        delete elem.second;
    }
}

HMENU MainWindow::MainWindowMenu()
{
    auto hFileMenu = CreateMenu();
    {
        (void)AppendMenu(hFileMenu, MF_STRING, IDM_APP_START, TEXT("Start"));
        (void)AppendMenu(hFileMenu, MF_STRING, IDM_APP_STOP, TEXT("Stop"));
        (void)AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr);
        (void)AppendMenu(hFileMenu, MF_STRING, IDM_APP_EXIT, TEXT("&Exit"));
    }

    auto hThreadMenu = CreateMenu();
    {
        (void)AppendMenu(hThreadMenu, MF_STRING, IDM_APP_START_SOCKET_THREAD, TEXT("Start"));
        (void)AppendMenu(hThreadMenu, MF_STRING, IDM_APP_STOP_SOCKET_THREAD, TEXT("Stop"));
    }

    auto hMenu = CreateMenu();
    {
        (void)AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, TEXT("&File"));
        (void)AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hThreadMenu, TEXT("&Socket Thread"));
    }

    return hMenu;
}

void MainWindow::MessageBox_Error(const TCHAR *errStr)
{
    (void)::MessageBox(nullptr, errStr, MainWindowName, MB_ICONERROR);
}

void MainWindow::Create(HINSTANCE hInstance, int iCmdShow)
{
    if (nullptr == Inst)
    {
        Inst = new MainWindow { hInstance, iCmdShow };
    }
}

void MainWindow::Show()
{
    WM_USER_START_Handler *pUserStartHandler = new WM_USER_START_Handler;

    Inst->m_WndProcMap[WM_CREATE]     = new WM_CREATE_Handler;
    Inst->m_WndProcMap[WM_SETFOCUS]   = new WM_SETFOCUS_Handler;
    Inst->m_WndProcMap[WM_SIZE]       = new WM_SIZE_Handler;
    Inst->m_WndProcMap[WM_USER_START] = pUserStartHandler;
    Inst->m_WndProcMap[WM_USER_STOP]  = new WM_USER_STOP_Handler;
    Inst->m_WndProcMap[MM_WIM_OPEN]   = new MM_WIM_OPEN_Handler(*pUserStartHandler);
    Inst->m_WndProcMap[MM_WIM_DATA]   = new MM_WIM_DATA_Handler;
    Inst->m_WndProcMap[MM_WIM_CLOSE]  = new MM_WIM_CLOSE_Handler(*pUserStartHandler);
    Inst->m_WndProcMap[WM_COMMAND]    = new WM_COMMAND_Handler;
    Inst->m_WndProcMap[WM_DESTROY]    = new WM_DESTROY_Handler;

    Inst->m_Wndclass.style         = (CS_HREDRAW | CS_VREDRAW);
    Inst->m_Wndclass.lpfnWndProc   = WndProc;
    Inst->m_Wndclass.cbClsExtra    = 0;
    Inst->m_Wndclass.cbWndExtra    = 0;
    Inst->m_Wndclass.hInstance     = Inst->m_hInstance;
    Inst->m_Wndclass.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    Inst->m_Wndclass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    Inst->m_Wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    Inst->m_Wndclass.lpszMenuName  = nullptr;
    Inst->m_Wndclass.lpszClassName = MainWindowName;

    auto RegisterClassResult = ::RegisterClass(&Inst->m_Wndclass);

    if (0 == RegisterClassResult)
    {
        MessageBox_Error(TEXT("Error in RegisterClass"));
        return;
    }

    Inst->m_hWindow = ::CreateWindow(
            MainWindowName,
            MainWindowName,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            MainWindowMenu(),
            Inst->m_hInstance,
            nullptr);

    if (Inst->m_hWindow == nullptr)
    {
        MessageBox_Error(TEXT("Error in CreateWindow"));
        return;
    }

    ShowWindow(Inst->m_hWindow, Inst->m_CmdShow);

    auto UpdateWindowResult = ::UpdateWindow(Inst->m_hWindow);

    if (UpdateWindowResult == FALSE)
    {
        MessageBox_Error(TEXT("Error in UpdateWindow"));
        return;
    }
}

void MainWindow::Run()
{
    MSG msg;
    auto msgres = ::GetMessage(&msg, nullptr, 0, 0);

    while ((msgres != 0) && (msgres != -1))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        msgres = ::GetMessage(&msg, nullptr, 0, 0);
    }

    if (msgres == -1)
    {
        MessageBox_Error(TEXT("Error in GetMessage"));
    }
}

void MainWindow::Destroy()
{
    delete Inst;
    Inst = nullptr;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (auto it = Inst->m_WndProcMap.find(message); it != Inst->m_WndProcMap.end())
    {
        WndProcHandler &handler { *it->second };

        auto result = handler(hwnd, wParam, lParam);

        return result;
    }
    else
    {
        return ::DefWindowProc(hwnd, message, wParam, lParam);
    }
}
