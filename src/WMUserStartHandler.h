#ifdef WM_USER_START_H
#error Already included
#else
#define WM_USER_START_H

class WMUserStartHandler: public WndProcHandler
{
    public:
        WMUserStartHandler();
        ~WMUserStartHandler();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    private:
        std::vector<BYTE> buffer1;
        std::vector<BYTE> buffer2;
        WAVEHDR WaveHdr1;
        WAVEHDR WaveHdr2;

    public:
        WMUserStartHandler(const WMUserStartHandler &instance) = delete;
        WMUserStartHandler(const WMUserStartHandler &&instance) = delete;
        WMUserStartHandler &operator=(const WMUserStartHandler &instance) = delete;
        WMUserStartHandler &operator=(const WMUserStartHandler &&instance) = delete;
};

#endif
