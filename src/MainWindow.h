#ifdef MAIN_WINDOW_H
#error Already included
#else
#define MAIN_WINDOW_H

class WndProcHandler;

class MainWindow
{
    public:
        ~MainWindow();

    public:
        static void Create(HINSTANCE hInstance, int iCmdShow);
        static void Show();
        static void Run();
        static void Destroy();
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        explicit MainWindow(HINSTANCE hInstance, int iCmdShow);

        static MainWindow *Inst;
        static const TCHAR MainWindowName[];

        static HMENU MainWindowMenu();
        static void MessageBox_Error(const TCHAR *errStr);

        const HINSTANCE m_hInstance;
        const int m_CmdShow;
        std::map<UINT, WndProcHandler*> m_WndProcMap;
        WNDCLASS m_Wndclass;
        HWND m_hWindow;

    public:
        MainWindow() = delete;
        MainWindow(const MainWindow &instance) = delete;
        MainWindow(const MainWindow &&instance) = delete;
        MainWindow &operator=(const MainWindow &instance) = delete;
        MainWindow &operator=(const MainWindow &&instance) = delete;
};

#endif
