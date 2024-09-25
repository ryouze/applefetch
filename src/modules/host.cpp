/**
 * @file host.cpp
 */

#include <algorithm>      // for std::remove_if
#include <array>          // for std::array
#include <cctype>         // for std::isspace, std::isdigit
#include <cstddef>        // for std::size_t
#include <exception>      // for std::exception
#include <stdexcept>      // for std::runtime_error
#include <string>         // for std::string, std::stoi, std::to_string
#include <sys/sysctl.h>   // for sysctl
#include <sys/time.h>     // for timeval, time_t, time, difftime
#include <sys/utsname.h>  // for utsname, uname

#include <fmt/core.h>

#include "core/env.hpp"
#include "core/shell.hpp"
#include "core/sysctl.hpp"
#include "host.hpp"

namespace modules::host {

// std::string get_hostname()
// {
//     struct utsname uts;
//     if (uname(&uts) != 0) {
//         throw std::runtime_error("Failed to get hostname using uname");
//     }
//     return uts.nodename;
// }

std::string get_version()
{
    try {
        return fmt::format("macOS {}", core::sysctl::get_value("kern.osproductversion"));
    }
    catch (const std::runtime_error &e) {
        return fmt::format("Unknown macOS version ({})", e.what());
    }
}

std::string get_model_identifier()
{
    try {
        return core::sysctl::get_value("hw.model");
    }
    catch (const std::runtime_error &e) {
        return fmt::format("Unknown model identifier ({})", e.what());
    }
}

std::string get_architecture()
{
    struct utsname uts;
    if (uname(&uts) != 0) {
        return "Unknown architecture (Failed to get uname)";
    }
    return uts.machine;
}

std::string get_uptime()
{
    struct timeval boottime;
    std::size_t len = sizeof(boottime);
    std::array<int, 2> mib = {CTL_KERN, KERN_BOOTTIME};

    if (sysctl(mib.data(), mib.size(), &boottime, &len, nullptr, 0) != 0) {
        return "Unknown uptime (Failed to get boottime)";
    }

    const time_t bsec = boottime.tv_sec;
    const time_t now = time(nullptr);

    const int seconds = static_cast<int>(difftime(now, bsec));
    const int days = seconds / (60 * 60 * 24);
    const int hours = (seconds % (60 * 60 * 24)) / (60 * 60);
    const int minutes = (seconds % (60 * 60)) / 60;

    return fmt::format("{}d {}h {}m", days, hours, minutes);
}

std::string get_packages()
{
    // Attempt to get the number of packages, assuming brew is installed
    if (auto output = core::shell::get_output("brew list | wc -l")) {
        // Trim whitespace from the output
        output->erase(std::remove_if(output->begin(), output->end(), std::isspace), output->cend());

        // Check if the output is a valid number
        if (!output->empty() && std::all_of(output->begin(), output->end(), std::isdigit)) {
            try {
                // Convert to integer and back to string to normalize
                const int package_count = std::stoi(output.value());
                return std::to_string(package_count);
            }
            catch (const std::exception &) {
                // If std::stoi throws an exception, proceed to check if brew is installed
                // This is unlikely after validation but included for safety
            }
        }
    }

    // If we reach here, the command failed or output was invalid
    // Now check if brew is installed
    if (core::shell::get_output("command -v brew")) {
        return "Unknown number of packages (Failed to get brew list)";
    }
    else {
        return "Unknown number of packages (Brew is not installed)";
    }
}

std::string get_shell()
{
    return core::env::get_variable("SHELL").value_or("Unknown shell");
}

}  // namespace modules::host
