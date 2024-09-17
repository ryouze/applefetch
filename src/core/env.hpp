/**
 * @file env.hpp
 *
 * @brief Get environment variables.
 */

#pragma once

#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

namespace core::env {

/**
 * @brief Exception raised by the environment.
 */
class EnvError : public std::runtime_error {
  public:
    explicit EnvError(const std::string &message)
        : std::runtime_error(message) {}
};

/**
 * @brief Get the value of an environment variable as a string.
 *
 * @param name Name of the environment variable (e.g., "SHELL").
 *
 * @return Value of the environment variable (e.g., "/bin/zsh").
 *
 * @throws EnvError If failed to get the environment variable.
 */
[[nodiscard]] std::string get_variable(const std::string &name);

}  // namespace core::env
