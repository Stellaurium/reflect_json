#pragma once

#include "macro_tool.h"
#include "macro_variable.h"

// count 的最大值是 64

#define __PP_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,     \
                        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
                        _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, \
                        _59, _60, _61, _62, _63, _64, N, ...)                                                          \
    N
#define PP_COUNT(...) PP_CALL(__PP_COUNT_IMPL, __VA_ARGS__ __VA_OPT__(, ) REVERSE_64_NUMBER)
