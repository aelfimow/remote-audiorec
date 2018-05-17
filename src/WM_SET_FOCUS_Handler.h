#ifdef WM_SETFOCUS_HANDLER_H
#error Already included
#else
#define WM_SETFOCUS_HANDLER_H

class WM_SETFOCUS_Handler: public WndProcHandler
{
    public:
        WM_SETFOCUS_Handler();
        ~WM_SETFOCUS_Handler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WM_SETFOCUS_Handler(const WM_SETFOCUS_Handler &instance) = delete;
        WM_SETFOCUS_Handler(const WM_SETFOCUS_Handler &&instance) = delete;
        WM_SETFOCUS_Handler &operator=(const WM_SETFOCUS_Handler &instance) = delete;
        WM_SETFOCUS_Handler &operator=(const WM_SETFOCUS_Handler &&instance) = delete;
};

#endif
