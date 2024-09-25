/**
 * @file host.cpp
 */

#include <array>          // for std::array
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
    try {
        return std::to_string(std::stoi(core::shell::get_output("brew list | wc -l")));
    }
    catch (const std::exception &) {  // Catch everything, as std::stoi can throw std::invalid_argument, std::out_of_range, or std::exception
        // Check if brew installed
        try {
            const std::string test = core::shell::get_output("command -v brew");
            return "Unknown number of packages (Failed to get brew list, but brew is installed)";
        }
        catch (const std::runtime_error &) {
            return "Unknown number of packages (Brew is not installed)";
        }
    }
}

std::string get_shell()
{
    try {
        return core::env::get_variable("SHELL");
    }
    catch (const std::runtime_error &e) {
        return fmt::format("Unknown shell ({})", e.what());
    }
}

}  // namespace modules::host
