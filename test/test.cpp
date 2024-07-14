#include "reflect_json.hpp"

#include <catch2/catch_all.hpp>
#include <fmt/format.h>

constexpr double eps = 1e-8;

TEST_CASE("test macro", "[macro]") {
    SECTION("test PP CALL") {
        REQUIRE_THAT(PP_CALL(sqrt, 6),
                     Catch::Matchers::WithinRel(sqrt(6), eps));
    }
    SECTION("test PP COUNT") {
        REQUIRE(PP_COUNT() == 0);
        REQUIRE(PP_COUNT(a) == 1);
        REQUIRE(PP_COUNT(1, s) == 2);
        REQUIRE(PP_COUNT(sadf, 123, 455) == 3);
        REQUIRE(PP_COUNT(sdf, 1231, 4123, 5234) == 4);
    }
    SECTION("test PP FOR EACH") {
        REQUIRE_THAT(
            PP_FOR_EACH(sqrt, PP_PLUS_SEPARATOR, 1, 4, 5, 6.9),
            Catch::Matchers::WithinRel(1 + 2 + sqrt(5) + sqrt(6.9), eps));
        REQUIRE_THAT(PP_FOR_EACH(sqrt, PP_PLUS_SEPARATOR, 1),
                     Catch::Matchers::WithinRel(1, eps));
        REQUIRE_THAT(PP_FOR_EACH(sqrt, PP_MULTIPLY_SEPARATOR, 1, 5, 2, 3, 5),
                     Catch::Matchers::WithinRel(5 * sqrt(6), eps));
    }
}

class NormalClassWithReflectInClass {
  public:
    std::string name;
    double balance;
    int age;
    std::vector<int> int_list;
    REFLECT_IN_CLASS(name, balance, age, int_list)
};

class NormalClassWithReflectOutClass {
  public:
    std::string name;
    double balance;
    int age;
    std::vector<int> int_list;
};
REFLECT_OUT_CLASS(NormalClassWithReflectOutClass, name, balance, age)

template <typename T>
class TemplateClassWithReflectInClass {
  public:
    std::string name;
    T value;
    REFLECT_IN_CLASS(name, value);
};

template <typename T>
class TemplateClassWithReflectOutClass {
  public:
    std::string name;
    T value;
};
REFLECT_OUT_CLASS_TEMPLATE((TemplateClassWithReflectOutClass<T>, typename T),
                           name, value);

template <typename T, typename W>
class Template2ClassWithReflectInClass {
  public:
    T a;
    W b;
    REFLECT_IN_CLASS(a, b);
};

template <typename T, typename W>
class Template2ClassWithReflectOutClass {
  public:
    T a;
    W b;
};
REFLECT_OUT_CLASS_TEMPLATE(((Template2ClassWithReflectOutClass<T, W>),
                            typename T, typename W),
                           a, b);

TEST_CASE("test reflect", "[reflect]") {
    auto in_class = NormalClassWithReflectInClass{"Way", 123.41, 54, {1, 2, 3}};
    auto in_class_string = serialize(in_class);
    fmt::println("{}", in_class_string);

    auto out_class = NormalClassWithReflectOutClass{"Jay", 12.6, 16, {3, 4, 1}};
    auto out_class_string = serialize(out_class);
    fmt::println("{}", out_class_string);

    auto template_in_class1 =
        TemplateClassWithReflectInClass{"template_in", 123.4};
    auto template_in_class1_string = serialize(template_in_class1);
    fmt::println("{}", template_in_class1_string);

    auto template_in_class2 =
        TemplateClassWithReflectOutClass<std::vector<int>>{"template_out",
                                                           {1, 2, 3, 4}};
    auto template_in_class2_string = serialize(template_in_class2);
    fmt::println("{}", template_in_class2_string);
}
