/**
 * @file cpu.cpp
 */

#include <optional>  // for std::optional
#include <string>    // for std::string

#include "core/sysctl.hpp"
#include "cpu.hpp"

namespace modules::cpu {

std::string get_cpu_model()
{
    if (const auto cpu_model_opt = core::sysctl::get_value("machdep.cpu.brand_string")) {
        return *cpu_model_opt;
    }
    else {
        return "Unknown CPU model (Failed to get machdep.cpu.brand_string)";
    }
}

}  // namespace modules::cpu
