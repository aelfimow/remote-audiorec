#ifdef WM_USER_STOP_HANDLER_H
#error Already included
#else
#define WM_USER_STOP_HANDLER_H

class WndProcParam;

class WM_USER_STOP_Handler: public WndProcHandler
{
    public:
        WM_USER_STOP_Handler();
        ~WM_USER_STOP_Handler();

        LRESULT operator()(const WndProcParam &param) override;

    public:
        WM_USER_STOP_Handler(const WM_USER_STOP_Handler &instance) = delete;
        WM_USER_STOP_Handler(const WM_USER_STOP_Handler &&instance) = delete;
        WM_USER_STOP_Handler &operator=(const WM_USER_STOP_Handler &instance) = delete;
        WM_USER_STOP_Handler &operator=(const WM_USER_STOP_Handler &&instance) = delete;
};

#endif
