#pragma once

#include <string>
#include <string_view>

namespace rot13 {

[[nodiscard]] std::string transform(std::string_view input);

}  // namespace rot13