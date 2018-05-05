#ifdef MAIN_WINDOW_H
#error Already included
#else
#define MAIN_WINDOW_H

class WndProcHandler;
class WMUserStartHandler;

class MainWindow
{
    public:
        ~MainWindow();

    public:
        static void Create();
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        MainWindow();
        static MainWindow *Inst;

        std::map<UINT, WndProcHandler*> m_WndProcMap;
        WMUserStartHandler * const m_WMUserStartHandler;

    public:
        MainWindow(const MainWindow &instance) = delete;
        MainWindow(const MainWindow &&instance) = delete;
        MainWindow &operator=(const MainWindow &instance) = delete;
        MainWindow &operator=(const MainWindow &&instance) = delete;
};

#endif
