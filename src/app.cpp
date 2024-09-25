/**
 * @file app.cpp
 */

#include <string>  // for std::string

#include <fmt/color.h>
#include <fmt/core.h>

#include "app.hpp"
#include "core/env.hpp"
#include "modules/cpu.hpp"
#include "modules/display.hpp"
#include "modules/host.hpp"
#include "modules/memory.hpp"

namespace app {

void run()
{
    // Check for NO_COLOR environment variable to determine if color should be disabled
    bool color_enabled = true;
    if (const auto no_color = core::env::get_variable("NO_COLOR"); no_color && !no_color->empty()) {
        color_enabled = false;
    }

    // Helper lambda to print with or without color
    const auto print_title = [color_enabled](const std::string &title) {
        if (color_enabled) {
            fmt::print(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, "{}: ", title);
        }
        else {
            fmt::print("{}: ", title);
        }
    };

    const auto print_value = [color_enabled](const std::string &value) {
        if (color_enabled) {
            fmt::print(fmt::fg(fmt::color::white), "{}\n", value);
        }
        else {
            fmt::print("{}\n", value);
        }
    };

    // Print system information with or without colors
    print_title("OS");
    print_value(fmt::format("{} ({})", modules::host::get_version(), modules::host::get_architecture()));

    print_title("Model");
    print_value(modules::host::get_model_identifier());

    print_title("Uptime");
    print_value(modules::host::get_uptime());

    print_title("Packages");
    print_value(fmt::format("{} (brew)", modules::host::get_packages()));

    print_title("Shell");
    print_value(modules::host::get_shell());

    print_title("Display");
    print_value(fmt::format("{} @ {}", modules::display::get_resolution(), modules::display::get_refresh_rate()));

    print_title("CPU");
    print_value(modules::cpu::get_cpu_model());

    print_title("Memory");
    print_value(modules::memory::get_memory_usage());
}

}  // namespace app
