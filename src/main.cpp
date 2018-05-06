#include <Windows.h>
#include <map>

#include "main.h"
#include "MainWindow.h"
#include "Console.h"

struct GlobalData G =
{
    nullptr,
    INVALID_HANDLE_VALUE,
    { },
    0,
    { },
    nullptr,
    FALSE,
    0
};

Console *console = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    hPrevInstance = hPrevInstance;
    szCmdLine = szCmdLine;

    MainWindow::Create(hInstance, iCmdShow);
    MainWindow::Show();
    MainWindow::Run();
    MainWindow::Destroy();

    return 0;
}
