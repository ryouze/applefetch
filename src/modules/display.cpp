/**
 * @file display.cpp
 */

#include <CoreGraphics/CoreGraphics.h>  // for CGDirectDisplayID, CGMainDisplayID, CGDisplayPixelsWide, CGDisplayPixelsHigh, CGDisplayCopyDisplayMode, CGDisplayModeGetRefreshRate, CGDisplayModeRelease
#include <cmath>                        // for std::round
#include <cstddef>                      // for std::size_t
#include <memory>                       // for std::unique_ptr
#include <string>                       // for std::string
#include <type_traits>                  // for std::remove_pointer_t

#include <fmt/core.h>

#include "display.hpp"

std::string modules::display::get_resolution()
{
    const CGDirectDisplayID display_id = CGMainDisplayID();
    const std::size_t width = CGDisplayPixelsWide(display_id);
    const std::size_t height = CGDisplayPixelsHigh(display_id);
    if (width == 0 || height == 0) {
        return "Unknown resolution (Both width and height are 0)";
    }
    return fmt::format("{}x{}", width, height);
}

std::string modules::display::get_refresh_rate()
{
    // Custom deleter for CGDisplayModeRef
    const auto cg_display_mode_deleter = [](const CGDisplayModeRef mode) {
        if (mode) {
            CGDisplayModeRelease(mode);
        }
    };

    const std::unique_ptr<std::remove_pointer_t<CGDisplayModeRef>, decltype(cg_display_mode_deleter)> mode(
        CGDisplayCopyDisplayMode(CGMainDisplayID()), cg_display_mode_deleter);

    if (!mode) {
        return "Unknown refresh rate (Failed to get display mode)";
    }

    // Return rounded refresh rate with " Hz" suffix
    const double refresh_rate = CGDisplayModeGetRefreshRate(mode.get());
    return fmt::format("{} Hz", static_cast<int>(std::round(refresh_rate)));
}
