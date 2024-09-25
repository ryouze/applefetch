/**
 * @file test_all.cpp
 */

#include <cstdlib>        // for EXIT_FAILURE, EXIT_SUCCESS
#include <exception>      // for std::exception
#include <functional>     // for std::function
#include <string>         // for std::string
#include <unordered_map>  // for std::unordered_map

#include <fmt/core.h>

#include "app.hpp"
#include "core/args.hpp"
#include "modules/cpu.hpp"
#include "modules/display.hpp"
#include "modules/host.hpp"
#include "modules/memory.hpp"

#define TEST_EXECUTABLE_NAME "tests"

namespace test_args {
[[nodiscard]] int none();
[[nodiscard]] int help();
[[nodiscard]] int version();
[[nodiscard]] int invalid();
}  // namespace test_args

namespace test_host {
[[nodiscard]] int get_version();
[[nodiscard]] int get_architecture();
[[nodiscard]] int get_model_identifier();
[[nodiscard]] int get_uptime();
[[nodiscard]] int get_packages();
[[nodiscard]] int get_shell();
}  // namespace test_host

namespace test_display {
[[nodiscard]] int get_resolution();
[[nodiscard]] int get_refresh_rate();
}  // namespace test_display

namespace test_cpu {
[[nodiscard]] int get_cpu_model();
}  // namespace test_cpu

namespace test_memory {
[[nodiscard]] int get_memory_usage();
}  // namespace test_memory

/**
 * @brief Entry-point of the test application.
 *
 * @param argc Number of command-line arguments (e.g., "2").
 * @param argv Array of command-line arguments (e.g., {"./bin", "-h"}).
 *
 * @return EXIT_SUCCESS if the test application ran successfully, EXIT_FAILURE otherwise.
 */
int main(int argc,
         char **argv)
{
    // Define the formatted help message
    const std::string help_message = fmt::format(
        "Usage: {} <test>\n"
        "\n"
        "Run automatic tests.\n"
        "\n"
        "Positional arguments:\n"
        "  test  name of the test to run ('all' to run all tests)\n",
        argv[0]);

    // If no arguments, print help message and exit
    if (argc == 1) {
        fmt::print("{}\n", help_message);
        return EXIT_FAILURE;
    }

    // Otherwise, define argument to function mapping
    const std::unordered_map<std::string, std::function<int()>> tests = {
        {"test_args::none", test_args::none},
        {"test_args::help", test_args::help},
        {"test_args::version", test_args::version},
        {"test_args::invalid", test_args::invalid},
        {"test_host::get_version", test_host::get_version},
        {"test_host::get_architecture", test_host::get_architecture},
        {"test_host::get_model_identifier", test_host::get_model_identifier},
        {"test_host::get_uptime", test_host::get_uptime},
        {"test_host::get_packages", test_host::get_packages},
        {"test_host::get_shell", test_host::get_shell},
        {"test_display::get_resolution", test_display::get_resolution},
        {"test_display::get_refresh_rate", test_display::get_refresh_rate},
        {"test_cpu::get_cpu_model", test_cpu::get_cpu_model},
        {"test_memory::get_memory_usage", test_memory::get_memory_usage},
    };

    // Get the test name from the command-line arguments
    const std::string arg = argv[1];

    // If the test name is found, run the corresponding test
    if (const auto it = tests.find(arg); it != tests.cend()) {
        try {
            return it->second();
        }
        catch (const std::exception &e) {
            fmt::print(stderr, "Test '{}' threw an exception: {}\n", arg, e.what());
            return EXIT_FAILURE;
        }
    }
    else if (arg == "all") {
        // Run all tests sequentially and print the results
        bool all_passed = true;
        for (const auto &[name, test_func] : tests) {
            fmt::print("Running test: {}\n", name);
            try {
                const int result = test_func();
                if (result != EXIT_SUCCESS) {
                    all_passed = false;
                    fmt::print(stderr, "Test '{}' failed.\n", name);
                }
                else {
                    fmt::print("Test '{}' passed.\n", name);
                }
            }
            catch (const std::exception &e) {
                all_passed = false;
                fmt::print(stderr, "Test '{}' threw an exception: {}\n", name, e.what());
            }
        }
        return all_passed ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else {
        fmt::print(stderr, "Error: Invalid test name: '{}'\n\n{}\n", arg, help_message);
        return EXIT_FAILURE;
    }
}

int test_args::none()
{
    try {
        char test_executable_name[] = TEST_EXECUTABLE_NAME;
        char *fake_argv[] = {test_executable_name};
        core::args::Args(1, fake_argv);
        // The program should run normally and print system information to console
        fmt::print("core::args::Args() passed: no arguments.\n");
        return EXIT_SUCCESS;
    }
    catch (const core::args::ArgsError &e) {
        fmt::print(stderr, "core::args::Args() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_args::help()
{
    try {
        char test_executable_name[] = TEST_EXECUTABLE_NAME;
        char arg_help[] = "-h";
        char *fake_argv[] = {test_executable_name, arg_help};
        core::args::Args(2, fake_argv);
        // This should never be reached, as the ArgsMessage exception should be thrown by the constructor
        fmt::print(stderr, "core::args::Args() failed: no help message displayed.\n");
        return EXIT_FAILURE;
    }
    catch (const core::args::ArgsMessage &) {
        fmt::print("core::args::Args() passed: help message displayed.\n");
        return EXIT_SUCCESS;
    }
}

int test_args::version()
{
    try {
        char test_executable_name[] = TEST_EXECUTABLE_NAME;
        char arg_version[] = "-v";
        char *fake_argv[] = {test_executable_name, arg_version};
        core::args::Args(2, fake_argv);
        // This should never be reached, as the ArgsMessage exception should be thrown by the constructor
        fmt::print(stderr, "core::args::Args() failed: no version displayed.\n");
        return EXIT_FAILURE;
    }
    catch (const core::args::ArgsMessage &e) {
        fmt::print("core::args::Args() passed: version displayed: {}\n", e.what());
        return EXIT_SUCCESS;
    }
}

int test_args::invalid()
{
    try {
        char test_executable_name[] = TEST_EXECUTABLE_NAME;
        char arg_hello[] = "hello";
        char *fake_argv[] = {test_executable_name, arg_hello};
        core::args::Args(2, fake_argv);
        // This should never be reached, as the ArgsError exception should be thrown by the constructor
        fmt::print(stderr, "core::args::Args() failed: invalid argument was not caught.\n");
        return EXIT_FAILURE;
    }
    catch (const core::args::ArgsError &e) {
        fmt::print("core::args::Args() passed: invalid argument caught: {}\n", e.what());
        return EXIT_SUCCESS;
    }
}

int test_host::get_version()
{
    try {
        const auto version = modules::host::get_version();
        if (version.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::host::get_version() failed: {}\n", version);
            return EXIT_FAILURE;  // Treat "Unknown" as failure unless explicitly expected
        }
        fmt::print("OS: {}\n", version);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::host::get_version() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_host::get_architecture()
{
    try {
        const auto architecture = modules::host::get_architecture();
        if (architecture.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::host::get_architecture() failed: {}\n", architecture);
            return EXIT_FAILURE;
        }
        fmt::print("Architecture: {}\n", architecture);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::host::get_architecture() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_host::get_model_identifier()
{
    try {
        const auto model = modules::host::get_model_identifier();
        if (model.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::host::get_model_identifier() failed: {}\n", model);
            return EXIT_FAILURE;
        }
        fmt::print("Model: {}\n", model);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::host::get_model_identifier() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_host::get_uptime()
{
    try {
        const auto uptime = modules::host::get_uptime();
        if (uptime.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::host::get_uptime() failed: {}\n", uptime);
            return EXIT_FAILURE;
        }
        fmt::print("Uptime: {}\n", uptime);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::host::get_uptime() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_host::get_packages()
{
    try {
        const auto packages = modules::host::get_packages();
        if (packages.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::host::get_packages() failed: {}\n", packages);
            return EXIT_FAILURE;
        }
        fmt::print("Packages: {} (brew)\n", packages);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::host::get_packages() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_host::get_shell()
{
    try {
        const auto shell = modules::host::get_shell();
        if (shell.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::host::get_shell() failed: {}\n", shell);
            return EXIT_FAILURE;
        }
        fmt::print("Shell: {}\n", shell);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::host::get_shell() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_display::get_resolution()
{
    try {
        const auto resolution = modules::display::get_resolution();
        if (resolution.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::display::get_resolution() failed: {}\n", resolution);
            return EXIT_FAILURE;
        }
        fmt::print("Resolution: {}\n", resolution);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::display::get_resolution() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_display::get_refresh_rate()
{
    try {
        const auto refresh_rate = modules::display::get_refresh_rate();
        if (refresh_rate.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::display::get_refresh_rate() failed: {}\n", refresh_rate);
            return EXIT_FAILURE;
        }
        fmt::print("Refresh rate: {}\n", refresh_rate);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::display::get_refresh_rate() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_cpu::get_cpu_model()
{
    try {
        const auto cpu_model = modules::cpu::get_cpu_model();
        if (cpu_model.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::cpu::get_cpu_model() failed: {}\n", cpu_model);
            return EXIT_FAILURE;
        }
        fmt::print("CPU: {}\n", cpu_model);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::cpu::get_cpu_model() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_memory::get_memory_usage()
{
    try {
        const auto memory_usage = modules::memory::get_memory_usage();
        if (memory_usage.find("Unknown") != std::string::npos) {
            fmt::print(stderr, "modules::memory::get_memory_usage() failed: {}\n", memory_usage);
            return EXIT_FAILURE;
        }
        fmt::print("Memory: {}\n", memory_usage);
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::memory::get_memory_usage() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}
