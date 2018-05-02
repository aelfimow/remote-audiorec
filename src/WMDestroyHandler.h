#ifdef WM_DESTROY_HANDLER_H
#error Already included
#else
#define WM_DESTROY_HANDLER_H

class WMDestroyHandler: public WndProcHandler
{
    public:
        WMDestroyHandler();
        ~WMDestroyHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WMDestroyHandler(const WMDestroyHandler &instance) = delete;
        WMDestroyHandler(const WMDestroyHandler &&instance) = delete;
        WMDestroyHandler &operator=(const WMDestroyHandler &instance) = delete;
        WMDestroyHandler &operator=(const WMDestroyHandler &&instance) = delete;
};

#endif
