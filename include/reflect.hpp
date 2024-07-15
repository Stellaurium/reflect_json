#pragma once

#include "macro_for_each.h"
#include "macro_variable.h"

#include <string>

// 该文件里面定义 reflect类 里面实现了 for_each_member_ptr
// 然后在封装一个函数 for_each_member

// =====================================
// 在类的内部使用，不需要指明类的名称，只需要写入需要序列化的内容即可
// 这里面的This类型需要从外面传入，但是反正也不会有人直接调用这个东西
// 都是在reflect这个结构体里面调用的，到那时候再指派This的类型
#define __REFLECT_CLASS_ELEMENT(name) func(#name, &This::name);

#define REFLECT_IN_CLASS(...)                                                                                          \
    template <typename This, typename Func>                                                                            \
    static void for_each_member_ptr(Func &&func) {                                                                     \
        PP_EXPAND(PP_FOR_EACH(__REFLECT_CLASS_ELEMENT, PP_ROUND_BRACKETS, PP_EMPTY_SEPARATOR, __VA_ARGS__))            \
    }

// =====================================
// template <__VA_ARGS__>  是给模板类服务的
// 是下面 Type 的参数
// 这里面需要 using This = PP_REMOVE_PARENTHESES_IF_EXIST(Type); 直接指定了 This
#define __REFLECT_OUT_CLASS_BEGIN(Type, ...)                                                                           \
    template <__VA_ARGS__>                                                                                             \
    struct reflect::__reflect_trait<PP_REMOVE_PARENTHESES_IF_EXIST(Type)> {                                            \
        using This = PP_REMOVE_PARENTHESES_IF_EXIST(Type);                                                             \
        static constexpr bool have_for_each_member_ptr() { return true; };                                             \
        template <typename Func>                                                                                       \
        static constexpr void for_each_member_ptr(Func &&func) {

#define __REFLECT_OUT_CLASS_END()                                                                                      \
    }                                                                                                                  \
    }                                                                                                                  \
    ;

// 在类的外部使用 需要手动指定type
// 所以Type可能不是一个参数，而是一个整体，包含了类型以及 模板参数
// 如果有多个参数 Type 必须加括号 视作一个整体
// 如果只有一个参数 可以不加括号
#define REFLECT_OUT_CLASS(Type, ...)                                                                                   \
    __REFLECT_OUT_CLASS_BEGIN(PP_REMOVE_PARENTHESES_IF_EXIST(Type))                                                    \
    PP_EXPAND(PP_FOR_EACH(__REFLECT_CLASS_ELEMENT, PP_ROUND_BRACKETS, PP_EMPTY_SEPARATOR, __VA_ARGS__))                \
    __REFLECT_OUT_CLASS_END()

// =====================================
namespace reflect {
// reflect_trait
// 形成一个统一的 for_each_member_ptr 接口
template <typename T>
struct __reflect_trait {
    static constexpr bool have_for_each_member_ptr() {
        return requires { T::for_each_member_ptr([](const std::string str, auto value) {}); };
    }

    template <typename Func>
    static constexpr void for_each_member_ptr(Func &&func) {
        // 他说要加上 template，不然就报错了
        // Use 'template' keyword to treat 'for_each_member_ptr' as a dependent template name
        T::template for_each_member_ptr<T>(std::forward<Func>(func));
    }
};

template <typename T, typename Func>
void for_each_member(const T &object, Func &&func) {
    __reflect_trait<T>::for_each_member_ptr([&](const std::string &key, auto ptr) { func(key, object.*ptr); });
}
} // namespace reflect