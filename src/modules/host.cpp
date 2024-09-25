/**
 * @file host.cpp
 */

#include <algorithm>      // for std::remove_if, std::all_of
#include <cctype>         // for std::isspace, std::isdigit
#include <cstddef>        // for std::size_t
#include <ctime>          // for std::time_t, std::time, std::difftime
#include <exception>      // for std::exception
#include <string>         // for std::string, std::to_string, std::stoi
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
    if (const auto version_opt = core::sysctl::get_value("kern.osproductversion")) {
        return fmt::format("macOS {}", *version_opt);
    }
    else {
        return "Unknown macOS version (Failed to get kern.osproductversion)";
    }
}

std::string get_model_identifier()
{
    if (const auto model_opt = core::sysctl::get_value("hw.model")) {
        return *model_opt;
    }
    else {
        return "Unknown model identifier (Failed to get hw.model)";
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
    const int mib[] = {CTL_KERN, KERN_BOOTTIME};
    const std::size_t mib_len = sizeof(mib) / sizeof(int);

    const auto boottime_opt = core::sysctl::get_value<struct timeval>(mib, mib_len);
    if (!boottime_opt) {
        return "Unknown uptime (Failed to get kern.boottime)";
    }

    const struct timeval boottime = *boottime_opt;
    const std::time_t bsec = boottime.tv_sec;
    const std::time_t now = std::time(nullptr);

    const int seconds = static_cast<int>(std::difftime(now, bsec));
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
        output->erase(std::remove_if(output->begin(), output->end(), std::isspace), output->end());

        // Check if the output is a valid number
        if (!output->empty() && std::all_of(output->begin(), output->end(), std::isdigit)) {
            try {
                // Convert to integer and back to string to normalize
                const int package_count = std::stoi(*output);
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
