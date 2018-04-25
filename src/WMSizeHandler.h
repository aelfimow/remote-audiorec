#ifdef WM_SIZE_HANDLER_H
#error Already included
#else
#define WM_SIZE_HANDLER_H

class WMSizeHandler: public WndProcHandler
{
    public:
        WMSizeHandler();
        ~WMSizeHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WMSizeHandler(const WMSizeHandler &instance) = delete;
        WMSizeHandler(const WMSizeHandler &&instance) = delete;
        WMSizeHandler &operator=(const WMSizeHandler &instance) = delete;
        WMSizeHandler &operator=(const WMSizeHandler &&instance) = delete;
};

#endif
