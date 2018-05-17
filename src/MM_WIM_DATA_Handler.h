#ifdef MM_WIM_DATA_HANDLER_H
#error Already included
#else
#define MM_WIM_DATA_HANDLER_H

class MM_WIM_DATA_Handler: public WndProcHandler
{
    public:
        MM_WIM_DATA_Handler();
        ~MM_WIM_DATA_Handler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        MM_WIM_DATA_Handler(const MM_WIM_DATA_Handler &instance) = delete;
        MM_WIM_DATA_Handler(const MM_WIM_DATA_Handler &&instance) = delete;
        MM_WIM_DATA_Handler &operator=(const MM_WIM_DATA_Handler &instance) = delete;
        MM_WIM_DATA_Handler &operator=(const MM_WIM_DATA_Handler &&instance) = delete;
};

#endif
