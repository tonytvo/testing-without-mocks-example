#pragma once

#include <string>
#include <string_view>

namespace rot13 {

/// Performs the ROT13 transformation on the provided input.
/// Letters in the ranges [A-Z] and [a-z] are rotated by 13 positions,
/// while all other characters are returned unchanged.
[[nodiscard]] std::string transform(std::string_view input);

}  // namespace rot13