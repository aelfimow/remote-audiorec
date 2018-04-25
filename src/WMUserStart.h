#ifdef WM_USER_START_H
#error Already included
#else
#define WM_USER_START_H

class WMUserStart: public WndProcHandler
{
    public:
        WMUserStart();
        ~WMUserStart();

        LRESULT operator()(HWND hwnd, WPARAM wParam, LPARAM lParam) override;

    private:
        std::vector<BYTE> buffer1;
        std::vector<BYTE> buffer2;
        WAVEHDR WaveHdr1;
        WAVEHDR WaveHdr2;

    public:
        WMUserStart(const WMUserStart &instance) = delete;
        WMUserStart(const WMUserStart &&instance) = delete;
        WMUserStart &operator=(const WMUserStart &instance) = delete;
        WMUserStart &operator=(const WMUserStart &&instance) = delete;
};

#endif
