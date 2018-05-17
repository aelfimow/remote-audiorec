#ifdef WM_DESTROY_HANDLER_H
#error Already included
#else
#define WM_DESTROY_HANDLER_H

class WM_DESTROY_Handler: public WndProcHandler
{
    public:
        WM_DESTROY_Handler();
        ~WM_DESTROY_Handler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WM_DESTROY_Handler(const WM_DESTROY_Handler &instance) = delete;
        WM_DESTROY_Handler(const WM_DESTROY_Handler &&instance) = delete;
        WM_DESTROY_Handler &operator=(const WM_DESTROY_Handler &instance) = delete;
        WM_DESTROY_Handler &operator=(const WM_DESTROY_Handler &&instance) = delete;
};

#endif
