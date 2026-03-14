#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace infrastructure {

class CommandLine {
public:
    static CommandLine create();
    static CommandLine createNull(std::vector<std::string> args = {}, std::ostream* output = nullptr);

    [[nodiscard]] const std::vector<std::string>& args() const noexcept;

private:
    CommandLine(std::vector<std::string> args, std::ostream* output);

    std::vector<std::string> args_{};
    std::ostream* output_{};
};

namespace command_line_detail {
void setProcessArgsForTesting(std::vector<std::string> args);
void resetProcessArgsForTesting();
const std::vector<std::string>& processArgs();
}  // namespace command_line_detail

}  // namespace infrastructure
