#pragma once
#include <ctime>
#include <string>
#include <array>

inline std::string makeDaytimeString()
{
    const time_t now = time(nullptr);
    std::array<char, 26> buf;
    if (const auto err = ctime_s(buf.data(), buf.size(), &now); err != 0)
    {
        return "Error";
    }
    return {buf.data()};
}
