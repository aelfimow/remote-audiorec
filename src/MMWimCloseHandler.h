#ifdef MM_WIM_CLOSE_HANDLER_H
#error Already included
#else
#define MM_WIM_CLOSE_HANDLER_H

class WMUserStartHandler;

class MMWimCloseHandler: public WndProcHandler
{
    public:
        MMWimCloseHandler(WMUserStartHandler &userStartHandler);
        ~MMWimCloseHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    private:
        WMUserStartHandler &m_UserStartHandler;

    public:
        MMWimCloseHandler() = delete;
        MMWimCloseHandler(const MMWimCloseHandler &instance) = delete;
        MMWimCloseHandler(const MMWimCloseHandler &&instance) = delete;
        MMWimCloseHandler &operator=(const MMWimCloseHandler &instance) = delete;
        MMWimCloseHandler &operator=(const MMWimCloseHandler &&instance) = delete;
};

#endif
