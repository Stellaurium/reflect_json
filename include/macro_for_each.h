#pragma once

// 这里面包含了所有自动生成的东西 不要格式化 一行一个最好了
#include "macro_for_each_impl.h"

// =====================================
// 定义一些工具

// 去掉括号，使用方法就是 PP_REMOVE_PARENTHESES variable
// 其中 variable 带着括号，就直接变成函数调用了
// #define PP_REMOVE_PARENTHESES(...) __VA_ARGS__

#define PP_EXPAND(x) x

// 这个定义是为了正确处理函数的参数的匹配
// 具体见Obsidian
// 解析宏调用时，宏参数不会展开。
// 解析宏调用后，宏定义文本中每个宏参数的使用都会替换为宏展开的参数，
// 但用于或操作`#`（`##`stringify 和 token paste）的宏参数除外，
// 这些宏参数会替换为宏参数的未展开文本。
// 然后执行`#`和`##`操作，然后再扫描一次整个宏体。
#define PP_CALL(func, ...) func(__VA_ARGS__)

// 定义两个是为了 适应c++宏的求值顺序
// 具体见obsidian中 "宏展开顺序"
// 简单来说
// 就是如果只有一层 PP_CONCAT(a, b) a##b
// 因为CONCAT里面有 # ## #@ 之一
// 就不会先计算内层的a和b
// 而直接将a b代表的东西拼接到一起
// 如果有两层那么第一个CONCAT里面就没有 # ## #@
// 所以先计算a和b
// 然后再计算CONCAT拼接到一起
#define __PP_CONCAT_IMPL(a, b) a##b
#define PP_CONCAT(a, b) __PP_CONCAT_IMPL(a, b)

#define PP_COMMA_SEPARATOR ,
#define PP_PLUS_SEPARATOR +
#define PP_MULTIPLY_SEPARATOR *
#define PP_SEMICOLON_SEPARATOR ;
#define PP_EMPTY_SEPARATOR

// ========================================
// PP_FOR_EACH
// 注意到这个 PP_FOR_EACH 不会自动生成 逗号
// 可能只能适用于这个场景
// 我们现在给 PP_FOR_EACH 添加新的参数，决定了函数的头是什么，函数的后面是什么
// 现在就可以自定义是空格作为分割符号 还是什么是分割符号
// TODO 现在PP_FOR_EACH只能变成圆括号，对于c++的尖括号无能为力

// TODO 有没有办法可以优化 PP_COUNT_IMPL2 中的那一串东西
// 方案一：PP_COUNT中需要套一层 而不能直接使用 PP_COUNT_IMPL2
// <del>原因应该与上面的 # 与 ## 类似，导致了展开顺序的问题
// 但是这个PP_COUNT_IMPL2并没有 # 或者 ##，所以原因不明
// 推测是有 ... 的宏函数 也不会展开进行参数求值 </del>
// 上面的推测是不正确的 错误发生在 函数参数匹配的过程中
// 函数参数匹配的时候不会进行展开
// 正确的原因是因为(__VA_ARGS__, REVERSE_64_NUMBER)里面只有__VA_ARGS__会进行展开
// 然后REVERSE_64_NUMBER并不会展开 会被直接匹配到 _i 参数中的一个
// 从而导致了错误
// 方案二： 使用CALL，来强制展开函数的所有参数
#define __PP_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ___PP_FOR_EACH16, _17, _18,  \
                        _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, \
                        _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, \
                        _57, _58, _59, _60, _61, _62, _63, _64, N, ...)                                                \
    N

// 注意 沙比 CLion 不会展开，他展开的可能是错误的
// 下面是两种方案
// #define PP_COUNT_IMPL1(...) PP_COUNT_IMPL2(__VA_ARGS__)
// #define PP_COUNT(...)
//  PP_COUNT_IMPL1(__VA_ARGS__ __VA_OPT__(, ) REVERSE_64_NUMBER)
#define PP_COUNT(...) PP_CALL(__PP_COUNT_IMPL, __VA_ARGS__ __VA_OPT__(, ) REVERSE_64_NUMBER)

// 以下两种方式都行，都是为了解决求值顺序的问题
#define PP_FOR_EACH(func, separator, ...)                                                                              \
    PP_CALL(__PP_CONCAT_IMPL, __PP_FOR_EACH_, PP_COUNT(__VA_ARGS__))                                                   \
    (func, separator, __VA_ARGS__)
// #define PP_FOR_EACH(func, separator, ...)
//   PP_CONCAT(__PP_FOR_EACH_, PP_COUNT(__VA_ARGS__))(func, separator,
//   __VA_ARGS__)
