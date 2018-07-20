#ifdef WM_SIZE_HANDLER_H
#error Already included
#else
#define WM_SIZE_HANDLER_H

class WndProcParam;

class WM_SIZE_Handler: public WndProcHandler
{
    public:
        WM_SIZE_Handler();
        ~WM_SIZE_Handler();

        LRESULT operator()(const WndProcParam &param) override;

    public:
        WM_SIZE_Handler(const WM_SIZE_Handler &instance) = delete;
        WM_SIZE_Handler(const WM_SIZE_Handler &&instance) = delete;
        WM_SIZE_Handler &operator=(const WM_SIZE_Handler &instance) = delete;
        WM_SIZE_Handler &operator=(const WM_SIZE_Handler &&instance) = delete;
};

#endif
