#include "infrastructure/CommandLine.hpp"

#include <ostream>


namespace infrastructure {

CommandLine CommandLine::createNull(std::vector<std::string> args, std::ostream* /*output*/)
{
    (void)args;
    CommandLine instance;
    return instance;
}


const std::vector<std::string>& CommandLine::arguments() const noexcept
{
    return args_;
}

}  // namespace infrastructure