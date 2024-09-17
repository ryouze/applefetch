/**
 * @file host.hpp
 *
 * @brief Get host information.
 */

#pragma once

#include <string>  // for std::string

namespace modules::host {

// /**
//  * @brief Get the hostname of the machine.
//  *
//  * @return Hostname string (e.g., "MacBook-Pro.local").
//  *
//  * @throws std::runtime_error If failed to get the hostname.
//  */
// [[nodiscard]] std::string get_hostname();

/**
 * @brief Get the macOS version.
 *
 * @return macOS version string (e.g., "macOS 14.6.1") if succeeded, "Unknown macOS version ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_version();

/**
 * @brief Get the Apple model identifier.
 *
 * @return Model identifier string (e.g., "MacBookPro18,3") if succeeded, "Unknown model identifier ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_model_identifier();

/**
 * @brief Get the system architecture.
 *
 * @return Architecture string (e.g., "arm64") if succeeded, "Unknown architecture ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_architecture();

/**
 * @brief Get the system uptime as a formatted string.
 *
 * @return Formatted uptime string (e.g., "17d 16h 25m") if succeeded, "Unknown uptime ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_uptime();

/**
 * @brief Get the number of brew packages installed.
 *
 * @return Number of brew packages installed (e.g., "139") if succeeded, "Unknown number of packages ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_packages();

/**
 * @brief Get the shell used by the user.
 *
 * @return Shell string (e.g., "/bin/zsh") if succeeded, "Unknown shell ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_shell();

}  // namespace modules::host
