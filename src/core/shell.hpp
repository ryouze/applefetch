/**
 * @file shell.hpp
 *
 * @brief Get output of shell commands.
 */

#pragma once

#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

namespace core::shell {

/**
 * @brief Exception raised by the shell.
 */
class ShellError : public std::runtime_error {
  public:
    explicit ShellError(const std::string &message)
        : std::runtime_error(message) {}
};

/**
 * @brief Get the output of a shell command as a string.
 *
 * @param command Command to run (e.g., "brew list | wc -l").
 *
 * @return Output of the command (e.g., "139").
 *
 * @throws ShellError If failed to run the command or the command returned no output.
 */
[[nodiscard]] std::string get_output(const std::string &command);

}  // namespace core::shell
