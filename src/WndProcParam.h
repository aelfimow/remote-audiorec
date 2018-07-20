#ifdef WND_PROC_PARAM_H
#error Already included
#else
#define WND_PROC_PARAM_H

class WndProcParam
{
    public:
        explicit WndProcParam(HWND hwnd, WPARAM wParam, LPARAM lParam);
        ~WndProcParam();

        HWND hwnd() const;
        WPARAM wParam() const;
        LPARAM lParam() const;

    private:
        const HWND m_hwnd;
        const WPARAM m_wParam;
        const LPARAM m_lParam;

    public:
        WndProcParam() = delete;
        WndProcParam(const WndProcParam &instance) = delete;
        WndProcParam(const WndProcParam &&instance) = delete;
        WndProcParam &operator=(const WndProcParam &instance) = delete;
        WndProcParam &operator=(const WndProcParam &&instance) = delete;
};

#endif
