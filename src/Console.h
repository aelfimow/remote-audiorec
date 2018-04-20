#ifdef CONSOLE_H
#error Already included
#else
#define CONSOLE_H

class Console
{
    public:
        explicit Console(HWND hwnd, HINSTANCE hInstance);
        ~Console();

        bool is_error() const;

        void SetFocus();
        void MoveWindow(size_t x0, size_t y0, size_t x1, size_t y1);

        Console& operator<<(const TCHAR *str);
        Console& operator<<(size_t value);

    public:
        static const TCHAR eol[];

    private:
        const HWND m_hwnd;
        bool m_error;

    public:
        Console() = delete;
        Console(const Console &instance) = delete;
        Console(const Console &&instance) = delete;
        Console &operator=(const Console &instance) = delete;
        Console &operator=(const Console &&instance) = delete;
};

#endif
