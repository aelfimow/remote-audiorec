#include <Windows.h>
#include <vector>
#include <map>

#include "main.h"
#include "MainWindow.h"
#include "WndProcHandler.h"
#include "WMSizeHandler.h"
#include "WMSetFocusHandler.h"
#include "WMCreateHandler.h"
#include "WMUserStartHandler.h"
#include "WMUserStopHandler.h"
#include "MMWimOpenHandler.h"
#include "MMWimDataHandler.h"
#include "MMWimCloseHandler.h"
#include "WMCommandHandler.h"
#include "WMDestroyHandler.h"


MainWindow *MainWindow::Inst = nullptr;

const TCHAR MainWindow::MainWindowName[] = TEXT("remote-audiorec");


MainWindow::MainWindow(HINSTANCE hInstance) :
    m_WndProcMap { },
    m_Wndclass { }
{
    WMUserStartHandler *pUserStartHandler = new WMUserStartHandler;

    m_WndProcMap[WM_CREATE]     = new WMCreateHandler;
    m_WndProcMap[WM_SETFOCUS]   = new WMSetFocusHandler;
    m_WndProcMap[WM_SIZE]       = new WMSizeHandler;
    m_WndProcMap[WM_USER_START] = pUserStartHandler;
    m_WndProcMap[WM_USER_STOP]  = new WMUserStopHandler;
    m_WndProcMap[MM_WIM_OPEN]   = new MMWimOpenHandler(*pUserStartHandler);
    m_WndProcMap[MM_WIM_DATA]   = new MMWimDataHandler;
    m_WndProcMap[MM_WIM_CLOSE]  = new MMWimCloseHandler(*pUserStartHandler);
    m_WndProcMap[WM_COMMAND]    = new WMCommandHandler;
    m_WndProcMap[WM_DESTROY]    = new WMDestroyHandler;

    m_Wndclass.style         = (CS_HREDRAW | CS_VREDRAW);
    m_Wndclass.lpfnWndProc   = WndProc;
    m_Wndclass.cbClsExtra    = 0;
    m_Wndclass.cbWndExtra    = 0;
    m_Wndclass.hInstance     = hInstance;
    m_Wndclass.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    m_Wndclass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    m_Wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    m_Wndclass.lpszMenuName  = nullptr;
    m_Wndclass.lpszClassName = MainWindowName;

    auto RegisterClassResult = ::RegisterClass(&m_Wndclass);

    if (0 == RegisterClassResult)
    {
        MessageBox_Error(TEXT("Error in RegisterClass"));
        return;
    }
}

MainWindow::~MainWindow()
{
    for (auto &elem: m_WndProcMap)
    {
        delete elem.second;
    }

    delete Inst;
}

void MainWindow::MessageBox_Error(const TCHAR *errStr)
{
    (void)::MessageBox(nullptr, errStr, MainWindowName, MB_ICONERROR);
}

void MainWindow::Create(HINSTANCE hInstance)
{
    if (nullptr == Inst)
    {
        Inst = new MainWindow { hInstance };
    }
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto it = Inst->m_WndProcMap.find(message);

    if (it == Inst->m_WndProcMap.end())
    {
        return ::DefWindowProc(hwnd, message, wParam, lParam);
    }

    WndProcHandler &handler { *it->second };

    auto result = handler(hwnd, wParam, lParam);

    return result;
}
