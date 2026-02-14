#include <catch2/catch_test_macros.hpp>

#include "logic/Rot13.hpp"

namespace {

TEST_CASE("rot13::transform returns empty string for empty input", "[rot13][logic]") {
    const std::string input;
    const auto result = rot13::transform(input);
    CHECK(result.empty());
}

}  // namespace