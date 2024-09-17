/**
 * @file display.hpp
 *
 * @brief Get display information.
 */

#pragma once

#include <string>  // for std::string

namespace modules::display {

/**
 * @brief Get the screen resolution as a string.
 *
 * @return Screen resolution (e.g., "1512x982") if succeeded, "Unknown resolution ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_resolution();

/**
 * @brief Get the screen refresh rate as an integer.
 *
 * @return Screen refresh rate (e.g., "120 Hz") if succeeded, "Unknown refresh rate ($REASON)" otherwise.
 */
[[nodiscard]] std::string get_refresh_rate();

}  // namespace modules::display
