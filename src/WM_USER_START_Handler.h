#ifdef WM_USER_START_H
#error Already included
#else
#define WM_USER_START_H

class WndProcParam;

class WM_USER_START_Handler: public WndProcHandler
{
    public:
        WM_USER_START_Handler();
        ~WM_USER_START_Handler();

        LRESULT operator()(const WndProcParam &param) override;

        void addAudioBuffer();
        void removeAudioBuffer();

    private:
        std::vector<BYTE> buffer1;
        std::vector<BYTE> buffer2;
        WAVEHDR WaveHdr1;
        WAVEHDR WaveHdr2;

    public:
        WM_USER_START_Handler(const WM_USER_START_Handler &instance) = delete;
        WM_USER_START_Handler(const WM_USER_START_Handler &&instance) = delete;
        WM_USER_START_Handler &operator=(const WM_USER_START_Handler &instance) = delete;
        WM_USER_START_Handler &operator=(const WM_USER_START_Handler &&instance) = delete;
};

#endif
