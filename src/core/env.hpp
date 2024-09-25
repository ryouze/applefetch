/**
 * @file env.hpp
 *
 * @brief Get environment variables.
 */

#pragma once

#include <string>  // for std::string

namespace core::env {

/**
 * @brief Get the value of an environment variable as a string.
 *
 * @param name Name of the environment variable (e.g., "SHELL").
 *
 * @return Value of the environment variable (e.g., "/bin/zsh").
 *
 * @throws std::runtime_error If failed to get the environment variable.
 */
[[nodiscard]] std::string get_variable(const std::string &name);

}  // namespace core::env
