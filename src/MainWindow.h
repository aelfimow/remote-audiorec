#ifdef MAIN_WINDOW_H
#error Already included
#else
#define MAIN_WINDOW_H

class MainWindow
{
    public:
        MainWindow();
        ~MainWindow();

    public:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:

    public:
        MainWindow(const MainWindow &instance) = delete;
        MainWindow(const MainWindow &&instance) = delete;
        MainWindow &operator=(const MainWindow &instance) = delete;
        MainWindow &operator=(const MainWindow &&instance) = delete;
};

#endif
