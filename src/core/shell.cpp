/**
 * @file shell.cpp
 */

#include <array>      // for std::array
#include <cstdio>     // for popen, pclose, FILE, fgets
#include <memory>     // for std::unique_ptr
#include <optional>   // for std::optional
#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

#include <fmt/core.h>

#include "shell.hpp"

namespace core::shell {

std::optional<std::string> get_output(const std::string &command)
{
    std::array<char, 128> buffer;
    std::string result;
    const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    // If failed to execute command, return nullopt
    if (!pipe) {
        return std::nullopt;
    }

    // Read output of command to string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // If empty string, return nullopt
    if (result.empty()) {
        return std::nullopt;
    }

    return result;
}

}  // namespace core::shell
