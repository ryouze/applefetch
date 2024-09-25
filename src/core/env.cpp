/**
 * @file env.cpp
 */

#include <cstdlib>   // for std::getenv
#include <optional>  // for std::optional, std::nullopt
#include <string>    // for std::string

#include "env.hpp"

namespace core::env {

std::optional<std::string> get_variable(const std::string &name)
{
    const char *value = std::getenv(name.c_str());
    if (!value) {
        return std::nullopt;
    }
    return std::string(value);
}

}  // namespace core::env
