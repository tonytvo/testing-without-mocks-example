#include "infrastructure/CommandLine.hpp"

#include <ostream>
#include <utility>
#include <vector>

namespace {
std::vector<std::string>& storedProcessArgs()
{
    static std::vector<std::string> args{};
    return args;
}
}  // namespace

namespace infrastructure {

CommandLine::CommandLine(std::vector<std::string> args, std::ostream* output)
    : args_(std::move(args))
    , output_(output)
{
}

CommandLine CommandLine::create()
{
    const auto& processArgs = command_line_detail::processArgs();
    std::vector<std::string> args;

    if (processArgs.size() > 2)
    {
        args.assign(processArgs.begin() + 2, processArgs.end());
    }

    return CommandLine(std::move(args), nullptr);
}

CommandLine CommandLine::createNull(std::vector<std::string> args, std::ostream* output)
{
    return CommandLine(std::move(args), output);
}

const std::vector<std::string>& CommandLine::args() const noexcept
{
    return args_;
}

namespace command_line_detail {

void setProcessArgsForTesting(std::vector<std::string> args)
{
    storedProcessArgs() = std::move(args);
}

void resetProcessArgsForTesting()
{
    storedProcessArgs().clear();
}

const std::vector<std::string>& processArgs()
{
    return storedProcessArgs();
}

}  // namespace command_line_detail

}  // namespace infrastructure
