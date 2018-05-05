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
        static void Create(HINSTANCE hInstance);
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        explicit MainWindow(HINSTANCE hInstance);

        HMENU CreateCustomMenu();
        void MessageBox_Error(const TCHAR *errStr);

        static MainWindow *Inst;
        static const TCHAR MainWindowName[];

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
