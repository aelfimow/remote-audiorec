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


MainWindow::MainWindow() :
    m_WndProcMap { }
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
}

MainWindow::~MainWindow()
{
    for (auto &elem: m_WndProcMap)
    {
        delete elem.second;
    }

    delete Inst;
}

void MainWindow::Create()
{
    if (nullptr == Inst)
    {
        Inst = new MainWindow;
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
