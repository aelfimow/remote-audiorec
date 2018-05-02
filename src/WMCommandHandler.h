#ifdef WM_COMMAND_HANDLER_H
#error Already included
#else
#define WM_COMMAND_HANDLER_H

class WMCommandHandler: public WndProcHandler
{
    public:
        WMCommandHandler();
        ~WMCommandHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        WMCommandHandler(const WMCommandHandler &instance) = delete;
        WMCommandHandler(const WMCommandHandler &&instance) = delete;
        WMCommandHandler &operator=(const WMCommandHandler &instance) = delete;
        WMCommandHandler &operator=(const WMCommandHandler &&instance) = delete;
};

#endif
