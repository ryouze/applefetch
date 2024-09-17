/**
 * @file sysctl.hpp
 *
 * @brief Get system information using sysctl.
 */

#pragma once

#include <cstddef>       // for std::size_t
#include <stdexcept>     // for std::runtime_error
#include <string>        // for std::string
#include <sys/sysctl.h>  // for sysctlbyname
#include <type_traits>   // for std::is_arithmetic_v

#include <fmt/core.h>  // for fmt::format

namespace core::sysctl {

/**
 * @brief Exception raised by the sysctl command.
 */
class SysCtlError : public std::runtime_error {
  public:
    explicit SysCtlError(const std::string &message)
        : std::runtime_error(message) {}
};

/**
 * @brief Get the value of a sysctl variable.
 *
 * Template function that handles various types of sysctl values.
 *
 * @tparam T Type of the sysctl value to retrieve (e.g., "std::uint64_t").
 * @param name Name of the sysctl variable (e.g., "hw.memsize").
 *
 * @return Value of the sysctl variable of the specified type (e.g., '17179869184").
 *
 * @throws SysCtlError If failed to get the sysctl value.
 */
template <typename T>
[[nodiscard]] inline T get_value(const std::string &name)
{
    // Compile-time check for arithmetic type
    static_assert(std::is_arithmetic_v<T>, "get_value() requires an arithmetic type");
    T value{};
    std::size_t size = sizeof(T);

    if (sysctlbyname(name.c_str(), &value, &size, nullptr, 0) != 0) {
        throw SysCtlError(fmt::format("Failed to get sysctl value for '{}'", name));
    }

    return value;
}

/**
 * @brief Get the value of a sysctl variable as a string.
 *
 * Overload specifically for std::string to handle string sysctl values.
 *
 * @param name Name of the sysctl variable (e.g., "kern.osproductversion").
 *
 * @return Value of the sysctl variable as a string (e.g., "14.6.1").
 *
 * @throws SysCtlError If failed to get the sysctl value.
 */
[[nodiscard]] inline std::string get_value(const std::string &name)
{
    std::size_t size;
    // First call with nullptr to determine required buffer size
    if (sysctlbyname(name.c_str(), nullptr, &size, nullptr, 0) != 0) {
        throw SysCtlError(fmt::format("Failed to get sysctl value size for '{}'", name));
    }

    // Allocate buffer based on required size
    std::string buffer(size, '\0');

    // Second call to get the actual data
    if (sysctlbyname(name.c_str(), buffer.data(), &size, nullptr, 0) != 0) {
        throw SysCtlError(fmt::format("Failed to get sysctl value for '{}'", name));
    }

    // Remove null terminator from buffer
    buffer.resize(size - 1);

    return buffer;
}

}  // namespace core::sysctl
