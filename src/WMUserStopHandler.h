#ifdef WM_USER_STOP_HANDLER_H
#error Already included
#else
#define WM_USER_STOP_HANDLER_H

class WM_USER_STOP_Handler: public WndProcHandler
{
    public:
        WM_USER_STOP_Handler();
        ~WM_USER_STOP_Handler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WM_USER_STOP_Handler(const WM_USER_STOP_Handler &instance) = delete;
        WM_USER_STOP_Handler(const WM_USER_STOP_Handler &&instance) = delete;
        WM_USER_STOP_Handler &operator=(const WM_USER_STOP_Handler &instance) = delete;
        WM_USER_STOP_Handler &operator=(const WM_USER_STOP_Handler &&instance) = delete;
};

#endif
