#include <catch2/catch_test_macros.hpp>

#include <string>
#include <vector>

#if __has_include("infrastructure/CommandLine.hpp")
#include "infrastructure/CommandLine.hpp"
#else
#include <ostream>
namespace infrastructure {
class CommandLine {
public:
    static CommandLine createNull(std::vector<std::string> args = {}, std::ostream* = nullptr)
    {
        (void)args;
        return CommandLine{};
    }

    [[nodiscard]] const std::vector<std::string>& arguments() const noexcept
    {
        return args_;
    }

private:
    std::vector<std::string> args_{};
};
}  // namespace infrastructure
#endif

TEST_CASE("CommandLine createNull stores provided arguments", "[command_line]")
{
    const std::vector<std::string> providedArgs{"first", "second", "third"};

    auto commandLine = infrastructure::CommandLine::createNull(providedArgs);

    REQUIRE(commandLine.arguments() == providedArgs);
}