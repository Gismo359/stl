#pragma once

#if CONFIG_RELEASE
#define assert(...) __builtin_assume(!!(__VA_ARGS__))
#else
#define assert(...) if (not (__VA_ARGS__)) __debugbreak()
#endif  