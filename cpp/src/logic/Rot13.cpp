#include "logic/Rot13.hpp"

namespace rot13 {

std::string transform(std::string_view input)
{
    std::string output(input);

    for (char& ch : output)
    {
        if (ch >= 'a' && ch <= 'z')
        {
            ch = static_cast<char>('a' + ((ch - 'a' + 13) % 26));
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = static_cast<char>('A' + ((ch - 'A' + 13) % 26));
        }
    }

    return output;
}

}  // namespace rot13