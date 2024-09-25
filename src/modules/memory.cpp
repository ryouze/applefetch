/**
 * @file memory.cpp
 */

#include <cstdint>      // for std::uint64_t
#include <mach/mach.h>  // for mach_port_t, mach_host_self, vm_size_t, vm_statistics64_data_t, mach_msg_type_number_t, host_statistics64, HOST_VM_INFO64, HOST_VM_INFO64_COUNT, KERN_SUCCESS
#include <string>       // for std::string

#include <fmt/core.h>

#include "core/sysctl.hpp"
#include "memory.hpp"

namespace modules::memory {

std::string get_memory_usage()
{
    // Fetch total physical memory using the sysctl abstraction
    const auto total_memory_opt = core::sysctl::get_value<std::uint64_t>("hw.memsize");
    if (!total_memory_opt) {
        return "Unknown memory usage (Failed to get hw.memsize)";
    }
    const std::uint64_t total_memory = *total_memory_opt;

    // Get page size
    const mach_port_t host_port = mach_host_self();
    vm_size_t page_size = 0;
    if (host_page_size(host_port, &page_size) != KERN_SUCCESS) {
        return "Unknown memory usage (Failed to get page size)";
    }

    // Fetch VM statistics
    vm_statistics64_data_t vm_stats{};
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

    if (host_statistics64(host_port, HOST_VM_INFO64, reinterpret_cast<host_info64_t>(&vm_stats), &count) != KERN_SUCCESS) {
        return "Unknown memory usage (Failed to get VM statistics)";
    }

    // Calculate used memory: active + wired + compressed
    const std::uint64_t used_memory = (static_cast<std::uint64_t>(vm_stats.active_count) +
                                       static_cast<std::uint64_t>(vm_stats.wire_count) +
                                       static_cast<std::uint64_t>(vm_stats.compressor_page_count)) *
                                      page_size;

    // Calculate used memory percentage
    const int used_memory_percentage = static_cast<int>((used_memory * 100) / total_memory);

    // Format the output as "<used_memory>GiB / <total_memory>GiB"
    return fmt::format("{:.2f}GiB / {:.2f}GiB ({}%)",
                       used_memory / (1024.0 * 1024.0 * 1024.0),
                       total_memory / (1024.0 * 1024.0 * 1024.0),
                       used_memory_percentage);
}

}  // namespace modules::memory
