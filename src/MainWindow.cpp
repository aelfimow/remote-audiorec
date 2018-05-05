#include <Windows.h>

#include "MainWindow.h"


MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return ::DefWindowProc(hwnd, message, wParam, lParam);
}
