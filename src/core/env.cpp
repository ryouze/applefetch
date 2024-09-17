/**
 * @file env.cpp
 */

#include <cstdlib>  // for std::getenv
#include <string>   // for std::string

#include <fmt/core.h>

#include "env.hpp"

std::string core::env::get_variable(const std::string &name)
{
    const char *value = std::getenv(name.c_str());
    if (!value) {
        throw core::env::EnvError(fmt::format("Environment variable not found: {}", name));
    }
    return std::string(value);
}
