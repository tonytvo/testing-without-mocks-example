#include <catch2/catch_test_macros.hpp>

#include <string>
#include <utility>
#include <vector>

#if __has_include("infrastructure/CommandLine.hpp")
#include "infrastructure/CommandLine.hpp"
#else
#include <ostream>
namespace infrastructure {
class CommandLine {
public:
    static CommandLine create()
    {
        return CommandLine{};
    }

    static CommandLine createNull(std::vector<std::string> args = {}, std::ostream* = nullptr)
    {
        CommandLine instance;
        instance.args_ = std::move(args);
        return instance;
    }

    [[nodiscard]] const std::vector<std::string>& args() const noexcept
    {
        return args_;
    }

private:
    std::vector<std::string> args_{};
};

namespace command_line_detail {
inline void setProcessArgsForTesting(std::vector<std::string>) {}
inline void resetProcessArgsForTesting() {}
}  // namespace command_line_detail

}  // namespace infrastructure
#endif

namespace {

class ScopedProcessArgs {
public:
    explicit ScopedProcessArgs(std::vector<std::string> args)
        : active_(true)
    {
        infrastructure::command_line_detail::setProcessArgsForTesting(std::move(args));
    }

    ScopedProcessArgs(const ScopedProcessArgs&) = delete;
    ScopedProcessArgs& operator=(const ScopedProcessArgs&) = delete;

    ~ScopedProcessArgs()
    {
        if (active_)
        {
            infrastructure::command_line_detail::resetProcessArgsForTesting();
        }
    }

    ScopedProcessArgs(ScopedProcessArgs&& other) noexcept
        : active_(other.active_)
    {
        other.active_ = false;
    }

private:
    bool active_;
};

}  // namespace

TEST_CASE("CommandLine create() skips program and script arguments", "[command_line]")
{
    const ScopedProcessArgs scopedArgs({"program", "script", "alpha", "beta"});

    const auto commandLine = infrastructure::CommandLine::create();

    REQUIRE(commandLine.args() == std::vector<std::string>{"alpha", "beta"});
}

TEST_CASE("CommandLine createNull defaults to no arguments", "[command_line]")
{
    const auto commandLine = infrastructure::CommandLine::createNull();

    REQUIRE(commandLine.args().empty());
}

TEST_CASE("CommandLine createNull stores provided arguments", "[command_line]")
{
    const std::vector<std::string> providedArgs{"first", "second", "third"};

    const auto commandLine = infrastructure::CommandLine::createNull(providedArgs);

    REQUIRE(commandLine.args() == providedArgs);
}
