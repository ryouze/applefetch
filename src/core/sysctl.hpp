/**
 * @file sysctl.hpp
 *
 * @brief Get system information using sysctl.
 */

#pragma once

#include <cstddef>       // for std::size_t
#include <optional>      // for std::optional
#include <string>        // for std::string
#include <sys/sysctl.h>  // for ::sysctl, ::sysctlbyname
#include <type_traits>   // for std::is_arithmetic_v, std::is_standard_layout_v, std::is_trivial_v

namespace core::sysctl {

/**
 * @brief Get the value of a sysctl variable of arithmetic type.
 *
 * Template function that handles various arithmetic types of sysctl values.
 *
 * @tparam T Type of the sysctl value to retrieve (e.g., "std::uint64_t").
 * @param name Name of the sysctl variable (e.g., "hw.memsize").
 *
 * @return Value if succeeded (e.g., '17179869184"), std::nullopt otherwise.
 */
template <typename T>
[[nodiscard]] inline std::optional<T> get_value(const std::string &name)
{
    // Compile-time check for arithmetic type
    static_assert(std::is_arithmetic_v<T>, "get_value() requires an arithmetic type");

    T value{};
    std::size_t size = sizeof(T);

    if (::sysctlbyname(name.c_str(), &value, &size, nullptr, 0) != 0) {
        return std::nullopt;
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
 * @return Value if succeeded (e.g., "14.6.1"), std::nullopt otherwise.
 */
[[nodiscard]] inline std::optional<std::string> get_value(const std::string &name)
{
    std::size_t size = 0;
    // First call with nullptr to determine required buffer size
    if (::sysctlbyname(name.c_str(), nullptr, &size, nullptr, 0) != 0) {
        return std::nullopt;
    }

    // Allocate buffer based on required size
    std::string buffer(size, '\0');

    // Second call to get the actual data
    if (::sysctlbyname(name.c_str(), buffer.data(), &size, nullptr, 0) != 0) {
        return std::nullopt;
    }

    // Remove null terminator if present
    if (!buffer.empty() && buffer.back() == '\0') {
        buffer.pop_back();
    }

    return buffer;
}

/**
 * @brief Get sysctl value using MIB array.
 *
 * Template function that handles sysctl values accessed via MIB arrays.
 *
 * @tparam T Type of the sysctl value to retrieve.
 * @param mib Pointer to MIB array.
 * @param mib_len Length of the MIB array.
 *
 * @return Value if succeeded (e.g., "14"), std::nullopt otherwise.
 */
template <typename T>
[[nodiscard]] inline std::optional<T> get_value(const int *mib,
                                                const std::size_t mib_len)
{
    // Ensure that T is a POD (Plain Old Data) type
    // A POD type in C++ is a type that is compatible with C-style data structures
    static_assert(std::is_standard_layout_v<T> && std::is_trivial_v<T>, "get_value() requires a POD type");

    T value{};
    std::size_t size = sizeof(T);

    if (::sysctl(const_cast<int *>(mib), static_cast<u_int>(mib_len), &value, &size, nullptr, 0) != 0) {
        return std::nullopt;
    }

    return value;
}

}  // namespace core::sysctl
