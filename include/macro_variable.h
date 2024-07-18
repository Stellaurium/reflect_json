#pragma once

// 常量定义
// 需要带一个尾巴 因为需要传入__PP_FOR_EACH_i在进行处理
// 如果在外面的话 如果是逗号 就见当作分割符了
#define PP_COMMA_SEPARATOR
#define PP_COMMA_SEPARATOR
#define PP_PLUS_SEPARATOR
#define PP_SEMICOLON_SEPARATOR
#define PP_EMPTY_SEPARATOR
#define __PP_COMMA_SEPARATOR_IMPL ,
#define __PP_PLUS_SEPARATOR_IMPL +
#define __PP_MULTIPLY_SEPARATOR_IMPL *
#define __PP_SEMICOLON_SEPARATOR_IMPL ;
#define __PP_EMPTY_SEPARATOR_IMPL


// brackets 同样延迟解析
#define PP_ROUND_BRACKETS
#define PP_SQUARE_BRACKETS
#define PP_CURLY_BRACKETS
#define PP_ANGLE_BRACKETS
#define __PP_ROUND_BRACKETS_BEGIN (
#define __PP_ROUND_BRACKETS_END )
#define __PP_SQUARE_BRACKETS_BEGIN [
#define __PP_SQUARE_BRACKETS_END ]
#define __PP_CURLY_BRACKETS_BEGIN {
#define __PP_CURLY_BRACKETS_END }
#define __PP_ANGLE_BRACKETS_BEGIN <
#define __PP_ANGLE_BRACKETS_END >

#define REVERSE_64_NUMBER                                                                                              \
    64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37,    \
        36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, \
        8, 7, 6, 5, 4, 3, 2, 1, 0

#define SEQUENTIAL_64_PARAMETER                                                                                        \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24,     \
        _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46,  \
        _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64
