/**
 * @file cpu.cpp
 */

#include <string>  // for std::string

#include <fmt/core.h>

#include "core/sysctl.hpp"
#include "cpu.hpp"

namespace modules::cpu {

std::string get_cpu_model()
{
    try {
        return core::sysctl::get_value("machdep.cpu.brand_string");
    }
    catch (const std::runtime_error &e) {
        return fmt::format("Unknown CPU model {}", e.what());
    }
}

}  // namespace modules::cpu
