#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace infrastructure {

class CommandLine {
public:
    static CommandLine createNull(std::vector<std::string> args = {}, std::ostream* output = nullptr);

    [[nodiscard]] const std::vector<std::string>& arguments() const noexcept;

private:
    std::vector<std::string> args_{};
};

}  // namespace infrastructure