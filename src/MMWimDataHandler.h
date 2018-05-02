#ifdef MM_WIM_DATA_HANDLER_H
#error Already included
#else
#define MM_WIM_DATA_HANDLER_H

class MMWimDataHandler: public WndProcHandler
{
    public:
        MMWimDataHandler();
        ~MMWimDataHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    public:
        MMWimDataHandler(const MMWimDataHandler &instance) = delete;
        MMWimDataHandler(const MMWimDataHandler &&instance) = delete;
        MMWimDataHandler &operator=(const MMWimDataHandler &instance) = delete;
        MMWimDataHandler &operator=(const MMWimDataHandler &&instance) = delete;
};

#endif
