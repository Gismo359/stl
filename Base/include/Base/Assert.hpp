#pragma once

#if CONFIG_RELEASE
    #define assert(...) __builtin_expect(!!(__VA_ARGS__), true)
#else
    #define assert(...)                                                                  \
        if (not(__VA_ARGS__))                                                            \
        {                                                                                \
            __debugbreak();                                                              \
        }
#endif