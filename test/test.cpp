#include "macro_count.h"
#include "macro_for_each.h"
#include "macro_tool.h"
#include "reflect.hpp"
#include "reflect_json.hpp"

#include <catch2/catch_all.hpp>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

constexpr double eps = 1e-8;
using json = nlohmann::json;

TEST_CASE("test macro", "[macro]") {
    SECTION("test PP CALL") { REQUIRE_THAT(PP_CALL(sqrt, 6), Catch::Matchers::WithinRel(sqrt(6), eps)); }
    SECTION("test PP COUNT") {
        REQUIRE(PP_COUNT() == 0);
        REQUIRE(PP_COUNT(a) == 1);
        REQUIRE(PP_COUNT(1, s) == 2);
        REQUIRE(PP_COUNT(sadf, 123, 455) == 3);
        REQUIRE(PP_COUNT(sdf, 1231, 4123, 5234) == 4);
    }
    SECTION("test PP FOR EACH") {
        REQUIRE_THAT(PP_FOR_EACH(sqrt, PP_ROUND_BRACKETS, PP_PLUS_SEPARATOR, 1, 4, 5, 6.9),
                     Catch::Matchers::WithinRel(1 + 2 + sqrt(5) + sqrt(6.9), eps));
        REQUIRE_THAT(PP_FOR_EACH(sqrt, PP_ROUND_BRACKETS, PP_PLUS_SEPARATOR, 1), Catch::Matchers::WithinRel(1, eps));
        REQUIRE_THAT(PP_FOR_EACH(sqrt, PP_ROUND_BRACKETS, PP_MULTIPLY_SEPARATOR, 1, 5, 2, 3, 5),
                     Catch::Matchers::WithinRel(5 * sqrt(6), eps));

        // std::tuple<int,double,std::string>
        std::tuple<PP_FOR_EACH(std::vector, PP_ANGLE_BRACKETS, PP_COMMA_SEPARATOR, int, double, std::string)> var{
            {42, 24}, {12.321}, {std::string("hello")}};

        REQUIRE(std::get<0>(var) == std::vector<int>{42, 24});
        REQUIRE_THAT(std::get<1>(var)[0], Catch::Matchers::WithinRel(12.321, eps));
        REQUIRE(std::get<2>(var) == std::vector<std::string>{"hello"});
    }
}

template <typename T>
class Address {
  public:
    T address;
    int count;
    // Constructor

    bool operator==(const Address &) const = default;
    REFLECT_IN_CLASS(address, count)
};

class NormalClassWithReflectInClass {
  public:
    std::string name;
    double balance;
    int age;
    std::vector<int> int_list;
    REFLECT_IN_CLASS(name, balance, age, int_list)
    bool operator==(const NormalClassWithReflectInClass &) const = default;
};

class NormalClassWithReflectOutClass {
  public:
    std::string name;
    double balance;
    int age;
    std::vector<int> int_list;
    bool operator==(const NormalClassWithReflectOutClass &) const = default;
};
REFLECT_OUT_CLASS(NormalClassWithReflectOutClass, name, balance, age, int_list)

#define IF_HAVE_FOR_EACH_MEMBER(...) reflect::__reflect_trait<__VA_ARGS__>::have_for_each_member_ptr()
TEST_CASE("test concept of has for_each_member", "[concept]") {
    CHECK(IF_HAVE_FOR_EACH_MEMBER(NormalClassWithReflectOutClass));
    CHECK(IF_HAVE_FOR_EACH_MEMBER(NormalClassWithReflectInClass));
    CHECK(!IF_HAVE_FOR_EACH_MEMBER(std::string));
    CHECK(!IF_HAVE_FOR_EACH_MEMBER(int));
    CHECK(!IF_HAVE_FOR_EACH_MEMBER(int &));
    CHECK(!IF_HAVE_FOR_EACH_MEMBER(int &&));
    CHECK(!IF_HAVE_FOR_EACH_MEMBER(std::vector<int>));
    CHECK(!IF_HAVE_FOR_EACH_MEMBER(std::map<std::string, int>));
}

TEST_CASE("test serialize") {
    SECTION("serialize") {
        REQUIRE(!reflect::__reflect_trait<std::string>::have_for_each_member_ptr());
        fmt::print("{}\n", serialize(123));
        serialize_json(std::string("hello"));
        auto s = std::string("hello");
        serialize_json(s);
        fmt::print("{}\n", serialize(std::string("hello")));
    }
    int a = 2;
    SECTION("deserialize") {
        REQUIRE(deserialize_json<int>(json(1)) == 1);
        REQUIRE(deserialize_json<int>(json(a)) == a);
        REQUIRE(deserialize_json<int>(serialize_json(a)) == a);
        REQUIRE(deserialize_json<int>(serialize_json(a)) == a);
    }
}

template <typename T>
class TemplateClassWithReflectInClass {
  public:
    std::string name;
    T value;
    REFLECT_IN_CLASS(name, value)
    bool operator==(const TemplateClassWithReflectInClass &) const = default;
};

template <typename T>
class TemplateClassWithReflectOutClass {
  public:
    std::string name;
    T value;
    bool operator==(const TemplateClassWithReflectOutClass &) const = default;
};
REFLECT_OUT_CLASS((TemplateClassWithReflectOutClass<T>, typename T), name, value)

template <typename T, typename W>
class Template2ClassWithReflectInClass {
  public:
    T a;
    W b;
    REFLECT_IN_CLASS(a, b)
    bool operator==(const Template2ClassWithReflectInClass &) const = default;
};

template <typename T, typename W>
class Template2ClassWithReflectOutClass {
  public:
    T a;
    W b;
    bool operator==(const Template2ClassWithReflectOutClass &) const = default;
};
REFLECT_OUT_CLASS(((Template2ClassWithReflectOutClass<T, W>), typename T, typename W), a, b)

template <typename T, typename W>
class Template2ClassWithSubClassWithReflectInClass {
  public:
    T a;
    W b;
    Address<std::string> address_in_class;
    bool operator==(const Template2ClassWithSubClassWithReflectInClass &) const = default;
    REFLECT_IN_CLASS(a, b, address_in_class)
};

template <typename T, typename W, typename U>
class Template2ClassWithSubClassWithReflectOutClass {
  public:
    T a;
    W b;
    Address<U> address_in_class;
    bool operator==(const Template2ClassWithSubClassWithReflectOutClass &) const = default;
};
REFLECT_OUT_CLASS(((Template2ClassWithSubClassWithReflectOutClass<T, W, U>), typename T, typename W, typename U), a, b,
                  address_in_class)

TEST_CASE("test reflect", "[reflect]") {

    SECTION("basic class") {
        auto in_class = NormalClassWithReflectInClass{"Way", 123.41, 54, {1, 2, 3}};
        auto in_class_string = serialize(in_class);
        fmt::println("{}", in_class_string);
        auto de_in_class = deserialize<decltype(in_class)>(in_class_string);
        CHECK(in_class == deserialize<decltype(in_class)>(in_class_string));
        CHECK(in_class.name == de_in_class.name);
        CHECK(in_class.age == de_in_class.age);
        CHECK(in_class.balance == de_in_class.balance);
        CHECK(in_class.int_list == de_in_class.int_list);

        auto out_class = NormalClassWithReflectOutClass{"Jay", 12.6, 16, {3, 4, 1}};
        auto out_class_string = serialize(out_class);
        fmt::println("{}", out_class_string);
        auto de_out_class = deserialize<NormalClassWithReflectOutClass>(out_class_string);
        REQUIRE(out_class == deserialize<decltype(out_class)>(out_class_string));
        CHECK(out_class.name == de_out_class.name);
        CHECK(out_class.age == de_out_class.age);
        CHECK(out_class.balance == de_out_class.balance);
        CHECK(out_class.int_list == de_out_class.int_list);
    }

    SECTION("1 para template") {
        auto template_in_class1 = TemplateClassWithReflectInClass<double>{"template_in", 123.4};
        auto template_in_class1_string = serialize(template_in_class1);
        fmt::println("{}", template_in_class1_string);
        REQUIRE(template_in_class1 == deserialize<decltype(template_in_class1)>(template_in_class1_string));

        auto template_in_class2 = TemplateClassWithReflectOutClass<std::vector<int>>{"template_out", {1, 2, 3, 4}};
        auto template_in_class2_string = serialize(template_in_class2);
        fmt::println("{}", template_in_class2_string);
        REQUIRE(template_in_class2 == deserialize<decltype(template_in_class2)>(template_in_class2_string));
    }

    SECTION("2 para template") {
        auto template2_in_class = Template2ClassWithReflectInClass<double, std::string>{12.3, "say"};
        auto template2_in_class_string = serialize(template2_in_class);
        fmt::println("{}", template2_in_class_string);
        REQUIRE(template2_in_class ==
                deserialize<Template2ClassWithReflectInClass<double, std::string>>(template2_in_class_string));

        auto template2_out_class = Template2ClassWithReflectOutClass<std::string, size_t>{"hello", 123};
        auto template2_out_class_string = serialize(template2_out_class);
        fmt::println("{}", template2_out_class_string);
        REQUIRE(template2_out_class == deserialize<decltype(template2_out_class)>(template2_out_class_string));
    }

    SECTION("sub class") {
        auto template2_sub_class_in_class = Template2ClassWithSubClassWithReflectInClass<std::string, size_t>{
            "hello", 123, Address<std::string>{"Beijing", 12}};
        auto template2_sub_class_in_class_string = serialize(template2_sub_class_in_class);
        fmt::println("{}", template2_sub_class_in_class_string);
        REQUIRE(template2_sub_class_in_class ==
                deserialize<decltype(template2_sub_class_in_class)>(template2_sub_class_in_class_string));

        auto template2_sub_class_out_class =
            Template2ClassWithSubClassWithReflectOutClass<std::string, size_t, std::string>{
                "hello", 123, {"Beijing", 12}};
        auto template2_sub_class_out_class_string = serialize(template2_sub_class_out_class);
        fmt::println("{}", template2_sub_class_out_class_string);
        REQUIRE(template2_sub_class_out_class ==
                deserialize<decltype(template2_sub_class_out_class)>(template2_sub_class_out_class_string));
    }
}
