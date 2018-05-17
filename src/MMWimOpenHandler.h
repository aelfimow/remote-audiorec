#ifdef MM_WIM_OPEN_HANDLER_H
#error Already included
#else
#define MM_WIM_OPEN_HANDLER_H

class WM_USER_START_Handler;

class MM_WIM_OPEN_Handler: public WndProcHandler
{
    public:
        MM_WIM_OPEN_Handler(WM_USER_START_Handler &userStartHandler);
        ~MM_WIM_OPEN_Handler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    private:
        WM_USER_START_Handler &m_UserStartHandler;

    public:
        MM_WIM_OPEN_Handler() = delete;
        MM_WIM_OPEN_Handler(const MM_WIM_OPEN_Handler &instance) = delete;
        MM_WIM_OPEN_Handler(const MM_WIM_OPEN_Handler &&instance) = delete;
        MM_WIM_OPEN_Handler &operator=(const MM_WIM_OPEN_Handler &instance) = delete;
        MM_WIM_OPEN_Handler &operator=(const MM_WIM_OPEN_Handler &&instance) = delete;
};

#endif
