#pragma once

#include "macro_for_each.h"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

//=========================================




#define REFLECT_OUT_CLASS()
#




// =====================================
// basic macro



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
