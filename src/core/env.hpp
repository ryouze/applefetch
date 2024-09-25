/**
 * @file env.hpp
 *
 * @brief Get environment variables.
 */

#pragma once

#include <optional>  // for std::optional
#include <string>    // for std::string

namespace core::env {

/**
 * @brief Get the value of an environment variable as a string.
 *
 * @param name Name of the environment variable (e.g., "SHELL").
 *
 * @return Value of the environment variable if succeeded (e.g., "/bin/zsh"), std::nullopt otherwise.
 *
 * @note This might be an empty string if the environment variable is set but empty.
 */
[[nodiscard]] std::optional<std::string> get_variable(const std::string &name);

}  // namespace core::env
