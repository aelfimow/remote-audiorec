#ifdef WM_SETFOCUS_HANDLER_H
#error Already included
#else
#define WM_SETFOCUS_HANDLER_H

class WMSetFocusHandler: public WndProcHandler
{
    public:
        WMSetFocusHandler();
        ~WMSetFocusHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WMSetFocusHandler(const WMSetFocusHandler &instance) = delete;
        WMSetFocusHandler(const WMSetFocusHandler &&instance) = delete;
        WMSetFocusHandler &operator=(const WMSetFocusHandler &instance) = delete;
        WMSetFocusHandler &operator=(const WMSetFocusHandler &&instance) = delete;
};

#endif
