#ifdef MM_WIM_CLOSE_HANDLER_H
#error Already included
#else
#define MM_WIM_CLOSE_HANDLER_H

class WM_USER_START_Handler;

class MM_WIM_CLOSE_Handler: public WndProcHandler
{
    public:
        MM_WIM_CLOSE_Handler(WM_USER_START_Handler &userStartHandler);
        ~MM_WIM_CLOSE_Handler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    private:
        WM_USER_START_Handler &m_UserStartHandler;

    public:
        MM_WIM_CLOSE_Handler() = delete;
        MM_WIM_CLOSE_Handler(const MM_WIM_CLOSE_Handler &instance) = delete;
        MM_WIM_CLOSE_Handler(const MM_WIM_CLOSE_Handler &&instance) = delete;
        MM_WIM_CLOSE_Handler &operator=(const MM_WIM_CLOSE_Handler &instance) = delete;
        MM_WIM_CLOSE_Handler &operator=(const MM_WIM_CLOSE_Handler &&instance) = delete;
};

#endif
