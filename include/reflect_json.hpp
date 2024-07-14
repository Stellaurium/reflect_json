#pragma once

#include "macro_for_each.h"

#include <nlohmann/json.hpp>

#include <string>
// =====================================

// basic macro
// __VA_ARGS__ 是下面 Type 的参数
// 因此可以实现在类外仍然可以定义一个 __REFLECT_OUT_CLASS_BEGIN 即可
// 如果类型是类似于 Type<double>的
// type<T> 会自动推导
#define __REFLECT_OUT_CLASS_BEGIN(Type, ...)                                     \
    template <__VA_ARGS__>                                                     \
    struct __reflect_trait<Type> {                                               \
        template <typename Func>                                               \
        static constexpr void for_each_member(Type &object, Func &&func) {

#define __REFLECT_OUT_CLASS_ELEMENT(element_name)                                \
    func(#element_name, object.element_name);

#define __REFLECT_OUT_CLASS_END()                                                \
    }                                                                          \
    }                                                                          \
    ;

//  在类的外部使用 需要手动指定type
#define REFLECT_OUT_CLASS(Type, ...)                                           \
    __REFLECT_OUT_CLASS_BEGIN(Type)                                            \
    PP_FOR_EACH(__REFLECT_OUT_CLASS_ELEMENT, PP_EMPTY_SEPARATOR, __VA_ARGS__)  \
    __REFLECT_OUT_CLASS_END()

// 由于希望支持在类的外部仍然可以 一次书写 实现模板的反射
// 所以Type可能不是一个参数，而是一个整体
// 这里面的TYPE必须加括号，且只能由一个括号，后面要用这个括号直接当函数的调用运算符
// 我们希望延迟展开这个整体，而不是最初就展开
#define REFLECT_OUT_CLASS_TEMPLATE(Type, ...)                                  \
    __REFLECT_OUT_CLASS_BEGIN Type PP_FOR_EACH(                              \
        __REFLECT_OUT_CLASS_ELEMENT,        \
                                             PP_EMPTY_SEPARATOR, __VA_ARGS__)            \
        __REFLECT_OUT_CLASS_END()

//  在类的内部使用，不需要指明类的名称，只需要写入需要序列化的内容即可

#define __REFLECT_IN_CLASS_BEGIN()                                               \
    template <typename Func>                                                   \
    void for_each_member(Func &&func) {

#define __REFLECT_IN_CLASS_ELEMENT(name) func(#name, this->name);

#define __REFLECT_IN_CLASS_END() }

#define REFLECT_IN_CLASS(...)                                                  \
    __REFLECT_IN_CLASS_BEGIN()                                                 \
    PP_FOR_EACH(__REFLECT_IN_CLASS_ELEMENT, PP_EMPTY_SEPARATOR, __VA_ARGS__)   \
    __REFLECT_IN_CLASS_END()

// =====================================
// basic template
// 用模板的特化来实现每一个类型的序列化方法
template <typename T>
struct __reflect_trait {
    template <typename Func>
    static constexpr void for_each_member(T &object, Func &&func) {
        object.for_each_member(std::forward<Func>(func));
    }
};

// 下面是写代码的过程中产生的没用的代码

// template <> struct __reflect_trait<Person> {
//   // 推荐使用万能引用来传递函数
//   // 因为函数包含 lambda 以及仿函数等
//   // copy可能是有消耗的
//   template <typename Func>
//   static void for_each_member(const Person &object, Func &&func) {
//     func("name", object.name);
//     func("age", object.age);
//     func("city", object.city);
//   }
// };

// __REFLECT_OUT_CLASS_BEGIN(Person)
// __REFLECT_OUT_CLASS_ELEMENT(name)
// __REFLECT_OUT_CLASS_ELEMENT(age)
// __REFLECT_OUT_CLASS_ELEMENT(city)
// __REFLECT_OUT_CLASS_END()

// REFLECT_OUT_CLASS(Person, name, age, city)

//  =====================================
// 序列化
using json = nlohmann::json;

template <typename T>
std::string serialize(T &object) {
    json root;
    __reflect_trait<T>::for_each_member(
        object,
        [&](const std::string &key, auto &value) { root[key] = value; });

    return root.dump(4);
}
