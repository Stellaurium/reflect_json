#pragma once

#include "macro_count.h"
#include "macro_for_each_impl.h" // 包含__PP_FOR_EACH_i的实现 太乱了 拿出来了
#include "macro_tool.h"
#include "macro_variable.h"

// __PP_FOR_EACH_i 参数如下
// __PP_FOR_EACH_i(func,brackets,separator,...)
#define PP_FOR_EACH(func, brackets, separator, ...)                                                                    \
    PP_CONCAT(__PP_FOR_EACH_, PP_COUNT(__VA_ARGS__))                                                                   \
    (func, brackets, separator, __VA_ARGS__)

