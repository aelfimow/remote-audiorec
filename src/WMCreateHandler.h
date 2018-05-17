#ifdef WM_CREATE_HANDLER_H
#error Already included
#else
#define WM_CREATE_HANDLER_H

class WM_CREATE_Handler: public WndProcHandler
{
    public:
        WM_CREATE_Handler();
        ~WM_CREATE_Handler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WM_CREATE_Handler(const WM_CREATE_Handler &instance) = delete;
        WM_CREATE_Handler(const WM_CREATE_Handler &&instance) = delete;
        WM_CREATE_Handler &operator=(const WM_CREATE_Handler &instance) = delete;
        WM_CREATE_Handler &operator=(const WM_CREATE_Handler &&instance) = delete;
};

#endif
