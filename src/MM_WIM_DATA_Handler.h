#ifdef MM_WIM_DATA_HANDLER_H
#error Already included
#else
#define MM_WIM_DATA_HANDLER_H

class WndProcParam;

class MM_WIM_DATA_Handler: public WndProcHandler
{
    public:
        MM_WIM_DATA_Handler();
        ~MM_WIM_DATA_Handler();

        LRESULT operator()(const WndProcParam &param) override;

    public:
        MM_WIM_DATA_Handler(const MM_WIM_DATA_Handler &instance) = delete;
        MM_WIM_DATA_Handler(const MM_WIM_DATA_Handler &&instance) = delete;
        MM_WIM_DATA_Handler &operator=(const MM_WIM_DATA_Handler &instance) = delete;
        MM_WIM_DATA_Handler &operator=(const MM_WIM_DATA_Handler &&instance) = delete;
};

#endif
