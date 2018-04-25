#ifdef WM_USER_STOP_HANDLER_H
#error Already included
#else
#define WM_USER_STOP_HANDLER_H

class WMUserStopHandler: public WndProcHandler
{
    public:
        WMUserStopHandler();
        ~WMUserStopHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WMUserStopHandler(const WMUserStopHandler &instance) = delete;
        WMUserStopHandler(const WMUserStopHandler &&instance) = delete;
        WMUserStopHandler &operator=(const WMUserStopHandler &instance) = delete;
        WMUserStopHandler &operator=(const WMUserStopHandler &&instance) = delete;
};

#endif
