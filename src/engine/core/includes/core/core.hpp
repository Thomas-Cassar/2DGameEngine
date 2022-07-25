#pragma once

/**
 * @brief If engineAssert returns false the program breaks
 */
#ifdef __linux__
#define engineAssert(x)                                                                                                \
    if (!(x))                                                                                                          \
        __builtin_trap();
#elif _WIN32
#define engineAssert(x)                                                                                                \
    if (!(x))                                                                                                          \
        __debugbreak();
#else
#define engineAssert(x)                                                                                                \
    if (!(x))                                                                                                          \
        exit(1);
#endif