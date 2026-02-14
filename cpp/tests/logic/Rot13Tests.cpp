#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>

#if __has_include("logic/Rot13.hpp")
#include "logic/Rot13.hpp"
#else
namespace rot13 {
    std::string transform(std::string_view input);
}
#endif

TEST_CASE("rot13 produces an empty string when given an empty string", "[rot13]")
{
    const std::string input{};
    const std::string expected{};

    const auto actual = rot13::transform(input);

    REQUIRE(actual == expected);
}