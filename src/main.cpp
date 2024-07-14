#include "reflect_json.hpp"

#include <fmt/format.h>

#include <iostream>
#include <string>

template <typename T>
class Person {
  public:
    std::string name;
    int age;
    std::string city;
    T balance;
    //    REFLECT_IN_CLASS(name, age, city, balance)
};
REFLECT_OUT_CLASS_TEMPLATE((Person<T>, typename T), name, age, city, balance)

int main() {
    auto person1 = Person<double>{
        .name = "John", .age = 30, .city = "New York", .balance = 12.0};
    auto person2 = Person<int>{
        .name = "Alex", .age = 30, .city = "Beijing", .balance = 15};

    fmt::print("{}\n", serialize(person1));
    fmt::print("{}\n", serialize(person2));

    auto var = deserialize<Person<double>>(serialize(person1));
    static_assert(std::is_same_v < double, decltype(var.balance)>);
    fmt::print("name: {}, age: {}, city: {}, balance: {}\n", var.name, var.age,
               var.city, var.balance);

    return 0;
}
