/**
 * @file cpu.hpp
 *
 * @brief Get CPU information.
 */

#pragma once

#include <string>  // for std::string

namespace modules::cpu {

/**
 * @brief Get the CPU model as a string.
 *
 * @return CPU model string (e.g., "Apple M1 Pro") if succeeded, "Unknown CPU model ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_cpu_model();

}  // namespace modules::cpu
