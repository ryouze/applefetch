/**
 * @file shell.cpp
 */

#include <array>   // for std::array
#include <cstdio>  // for popen, pclose, FILE, fgets
#include <memory>  // for std::unique_ptr
#include <string>  // for std::string

#include <fmt/core.h>

#include "shell.hpp"

namespace core::shell {

std::string get_output(const std::string &command)
{
    std::array<char, 128> buffer;
    std::string result;
    const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    // If failed to execute command, throw
    if (!pipe) {
        throw std::runtime_error(fmt::format("Failed to execute command: {}", command));
    }

    // Read output of command to string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // If empty string, throw
    if (result.empty()) {
        throw std::runtime_error(fmt::format("Command '{}' returned no output", command));
    }
    return result;
}

}  // namespace core::shell
