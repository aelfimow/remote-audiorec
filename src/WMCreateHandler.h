#ifdef WM_CREATE_HANDLER_H
#error Already included
#else
#define WM_CREATE_HANDLER_H

class WMCreateHandler: public WndProcHandler
{
    public:
        WMCreateHandler();
        ~WMCreateHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WMCreateHandler(const WMCreateHandler &instance) = delete;
        WMCreateHandler(const WMCreateHandler &&instance) = delete;
        WMCreateHandler &operator=(const WMCreateHandler &instance) = delete;
        WMCreateHandler &operator=(const WMCreateHandler &&instance) = delete;
};

#endif
