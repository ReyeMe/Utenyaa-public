#pragma once
#include "jo\Jo.hpp"
#include <stdint-gcc.h>

class Debug
{
private:
    static inline uint8_t line = 0;
    static inline uint8_t logCounter = 0;

    static auto GetNextLineToPrint()
    {
        if (line > 26)
        {
            line = 0;
        }

        jo_clear_screen_line(line);

        return line++;
    }

public:

#ifdef ENABLE_DEBUG
    static constexpr bool Enabled = true;
#else
    static constexpr bool Enabled = false;
#endif

    template <typename ...Args>
    static void LogLine(Args...args)
    {
        if constexpr (Enabled)
        {
            auto line = GetNextLineToPrint();
            jo_printf(0, line, "%d", logCounter++);
            jo_printf(4, line, args ...);
        }
    }

    template <typename ...Args>
    static void LogLineAndBreak(Args...args)
    {
        if constexpr (Enabled)
        {
            LogLine(args ...);
            jo_core_suspend();
        }
    }
};
