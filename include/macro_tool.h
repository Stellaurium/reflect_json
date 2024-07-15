#pragma once

#define PP_EXPAND(...) __VA_ARGS__

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

// ===============
// 如果有括号就去除括号
// 这个实在是太精妙了
#define PP_REMOVE_PARENTHESES_IF_EXIST(X) __PP_REMOVE_PARENTHESES_IMPL(__PP_ISH X)
#define __PP_ISH(...) __PP_ISH __VA_ARGS__
// 套一层保证优先级
#define __PP_REMOVE_PARENTHESES_IMPL(...) __PP_REMOVE_PARENTHESES_IMPL2(__VA_ARGS__)
#define __PP_REMOVE_PARENTHESES_IMPL2(...) __PP_VAN##__VA_ARGS__
#define __PP_VAN__PP_ISH // VANISH
