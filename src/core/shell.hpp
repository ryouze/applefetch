/**
 * @file shell.hpp
 *
 * @brief Get output of shell commands.
 */

#pragma once

#include <optional>  // for std::optional
#include <string>    // for std::string

namespace core::shell {

/**
 * @brief Get the output of a shell command as a string.
 *
 * @param command Command to run (e.g., "brew list | wc -l").
 *
 * @return Output of the command if succeeded (e.g., "139"), std::nullopt otherwise.
 *
 * @note std::nullopt is returned if the command fails to execute or the output is empty.
 */
[[nodiscard]] std::optional<std::string> get_output(const std::string &command);

}  // namespace core::shell
