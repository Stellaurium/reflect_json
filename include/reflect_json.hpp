#pragma once

#include "macro_for_each.h"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
// =====================================

// 这个实在是太精妙了
#define PP_REMOVE_PARENTHESES_IF_EXIST(X) __PP_REMOVE_PARENTHESES_IMPL(__PP_ISH X)
#define __PP_ISH(...) __PP_ISH __VA_ARGS__
// 套一层保证优先级
#define __PP_REMOVE_PARENTHESES_IMPL(...) __PP_REMOVE_PARENTHESES_IMPL2(__VA_ARGS__)
#define __PP_REMOVE_PARENTHESES_IMPL2(...) __PP_VAN##__VA_ARGS__
#define __PP_VAN__PP_ISH // VANISH

// basic macro
// __VA_ARGS__ 是下面 Type 的参数
// 因此可以实现在类外仍然可以定义一个 __REFLECT_OUT_CLASS_BEGIN 即可
// 如果类型是类似于 Type<double>的
// type<T> 会自动推导
#define __REFLECT_OUT_CLASS_BEGIN(Type, ...)                                                                           \
    template <__VA_ARGS__>                                                                                             \
    struct __reflect_trait<PP_REMOVE_PARENTHESES_IF_EXIST(Type)> {                                                     \
        static constexpr bool have_for_each_member() { return true; };                                                 \
        template <typename Func>                                                                                       \
        static constexpr void for_each_member(const PP_REMOVE_PARENTHESES_IF_EXIST(Type) & c_object, Func &&func) {    \
            auto &object = const_cast<PP_REMOVE_PARENTHESES_IF_EXIST(Type) &>(c_object);

#define __REFLECT_OUT_CLASS_ELEMENT(element_name) func(#element_name, object.element_name);

#define __REFLECT_OUT_CLASS_END()                                                                                      \
    }                                                                                                                  \
    }                                                                                                                  \
    ;

//  在类的外部使用 需要手动指定type
#define REFLECT_OUT_CLASS(Type, ...)                                                                                   \
    __REFLECT_OUT_CLASS_BEGIN(Type)                                                                                    \
    PP_FOR_EACH(__REFLECT_OUT_CLASS_ELEMENT, PP_EMPTY_SEPARATOR, __VA_ARGS__)                                          \
    __REFLECT_OUT_CLASS_END()

// 由于希望支持在类的外部仍然可以 一次书写 实现模板的反射
// 所以Type可能不是一个参数，而是一个整体
// 这里面的TYPE必须加括号，且只能由一个括号，后面要用这个括号直接当函数的调用运算符
// 我们希望延迟展开这个整体，而不是最初就展开
#define REFLECT_OUT_CLASS_TEMPLATE(Type, ...)                                                                          \
    __REFLECT_OUT_CLASS_BEGIN Type PP_FOR_EACH(__REFLECT_OUT_CLASS_ELEMENT, PP_EMPTY_SEPARATOR, __VA_ARGS__)           \
        __REFLECT_OUT_CLASS_END()

//  在类的内部使用，不需要指明类的名称，只需要写入需要序列化的内容即可

#define __REFLECT_IN_CLASS_BEGIN()                                                                                     \
    template <typename Func>                                                                                           \
    void for_each_member(Func &&func) const {                                                                          \
        auto non_const_this = const_cast<std::remove_const_t<std::remove_pointer_t<decltype(this)>> *>(this);

#define __REFLECT_IN_CLASS_ELEMENT(name) func(#name, non_const_this->name);

#define __REFLECT_IN_CLASS_END() }

#define REFLECT_IN_CLASS(...)                                                                                          \
    __REFLECT_IN_CLASS_BEGIN()                                                                                         \
    PP_FOR_EACH(__REFLECT_IN_CLASS_ELEMENT, PP_EMPTY_SEPARATOR, __VA_ARGS__)                                           \
    __REFLECT_IN_CLASS_END()

// =====================================
// basic template
// 用模板的特化来实现每一个类型的序列化方法
template <typename T>
struct __reflect_trait {
    static constexpr bool have_for_each_member() {
        return requires(T t) { t.for_each_member([](const std::string str, auto &value) {}); };
    }

    template <typename Func>
    static constexpr void for_each_member(const T &c_object, Func &&func) {
        auto &object = const_cast<T &>(c_object);
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

// Serialization for other types
template <typename T>
    requires(!__reflect_trait<T>::have_for_each_member())
json serialize_json(const T &object) {
    return object;
}

// Serialization for types with for_each_member
template <typename T>
    requires(__reflect_trait<T>::have_for_each_member())
json serialize_json(const T &object) {
    json root;
    __reflect_trait<T>::for_each_member(
        object, [&](const std::string &key, auto &value) { root[key] = serialize_json(value); });
    return root;
}

template <typename T>
std::string serialize(const T &object) {
    return serialize_json<std::decay_t<T>>(object).dump(4);
}

template <typename T>
    requires(!__reflect_trait<T>::have_for_each_member())
T deserialize_json(const json &json_value) {
    return json_value.get<std::decay_t<T>>();
}

template <typename T>
    requires(__reflect_trait<T>::have_for_each_member())
std::decay_t<T> deserialize_json(const json &json_value) {
    std::decay_t<T> object;

    __reflect_trait<std::decay_t<T>>::for_each_member(object, [&](const std::string &key, auto &value) {
        if (json_value.contains(key)) {
            // 这个的返回值可能是一个json
            value = deserialize_json<std::decay_t<decltype(value)>>(json_value[key]);
        }
    });

    return object;
}

template <typename T>
std::decay_t<T> deserialize(const std::string &stream) {
    json root = json::parse(stream);
    return deserialize_json<std::decay_t<T>>(root);
}
