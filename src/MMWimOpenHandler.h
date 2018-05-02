#ifdef MM_WIM_OPEN_HANDLER_H
#error Already included
#else
#define MM_WIM_OPEN_HANDLER_H

class WMUserStartHandler;

class MMWimOpenHandler: public WndProcHandler
{
    public:
        MMWimOpenHandler(WMUserStartHandler &userStartHandler);
        ~MMWimOpenHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    private:
        WMUserStartHandler &m_UserStartHandler;

    public:
        MMWimOpenHandler() = delete;
        MMWimOpenHandler(const MMWimOpenHandler &instance) = delete;
        MMWimOpenHandler(const MMWimOpenHandler &&instance) = delete;
        MMWimOpenHandler &operator=(const MMWimOpenHandler &instance) = delete;
        MMWimOpenHandler &operator=(const MMWimOpenHandler &&instance) = delete;
};

#endif
