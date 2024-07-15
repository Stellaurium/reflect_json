#pragma once

#include "reflect.hpp"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

//  =====================================
// 序列化

//==============
// serialize

// Serialization for other types
template <typename T>
    requires(!reflect::__reflect_trait<T>::have_for_each_member_ptr())
nlohmann::json serialize_json(const T &object) {
    return object;
}

// Serialization for types with for_each_member
template <typename T>
    requires(reflect::__reflect_trait<T>::have_for_each_member_ptr())
nlohmann::json serialize_json(const T &object) {
    nlohmann::json root;
    reflect::for_each_member(object, [&](const std::string &key, auto &value) { root[key] = serialize_json(value); });
    return root;
}

template <typename T>
std::string serialize(const T &object) {
    return serialize_json<std::decay_t<T>>(object).dump(4);
}

//==============
// deserialize
template <typename T>
    requires(!reflect::__reflect_trait<T>::have_for_each_member_ptr())
T deserialize_json(const nlohmann::json &json_value) {
    return json_value.get<std::decay_t<T>>();
}

template <typename T>
    requires(reflect::__reflect_trait<T>::have_for_each_member_ptr())
std::decay_t<T> deserialize_json(const nlohmann::json &json_value) {
    std::decay_t<T> object;

    reflect::for_each_member(object, [&](const std::string &key, auto &value) {
        // 这个的返回值可能是一个json
        value = deserialize_json<std::decay_t<decltype(value)>>(json_value[key]);
    });

    return object;
}

template <typename T>
std::decay_t<T> deserialize(const std::string &stream) {
    nlohmann::json root = nlohmann::json::parse(stream);
    return deserialize_json<std::decay_t<T>>(root);
}
