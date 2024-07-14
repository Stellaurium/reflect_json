#include "reflect_json.hpp"

#include <catch2/catch_all.hpp>

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
    //    std::vector<int> int_list;
    REFLECT_IN_CLASS(name, balance, age)
};
class NormalClassWithReflectOutClass {
  public:
    std::string name;
    double balance;
    int age;
    //    std::vector<int> int_list;
};

TEST_CASE("test reflect", "[reflect]") {}
