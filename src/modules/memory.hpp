/**
 * @file memory.hpp
 *
 * @brief Get memory information.
 */

#pragma once

#include <string>  // for std::string

namespace modules::memory {

/**
 * @brief Get memory usage as a formatted string (used / total).
 *
 * @return Formatted memory usage string (e.g., "11.14GiB / 16.00GiB") if succeeded, "Unknown memory usage ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_memory_usage();

}  // namespace modules::memory
