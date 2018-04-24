#ifdef WND_PROC_HANDLER_H
#error Already included
#else
#define WND_PROC_HANDLER_H

class WndProcHandler
{
    public:
        WndProcHandler() { }
        virtual ~WndProcHandler() { }

        virtual LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) = 0;

    public:
        WndProcHandler(const WndProcHandler &instance) = delete;
        WndProcHandler(const WndProcHandler &&instance) = delete;
        WndProcHandler &operator=(const WndProcHandler &instance) = delete;
        WndProcHandler &operator=(const WndProcHandler &&instance) = delete;
};

#endif
