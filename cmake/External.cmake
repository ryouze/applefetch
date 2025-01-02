include(FetchContent)

function(fetch_and_link_external_dependencies target)
  if(NOT TARGET ${target})
    message(FATAL_ERROR "[ERROR] Target '${target}' does not exist. Cannot fetch and link dependencies.")
  endif()

  set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
  set(FETCHCONTENT_QUIET OFF)
  set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/deps")

  # SYSTEM is used to prevent applying compile flags to the dependencies
  FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/releases/download/11.1.1/fmt-11.1.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
  )

  # Make dependencies available
  FetchContent_MakeAvailable(fmt)

  # Link dependencies to the target
  target_link_libraries(${target} PUBLIC fmt::fmt "-framework CoreGraphics")
  message(STATUS "[INFO] Linked dependencies 'fmt' and 'CoreGraphics' to target '${target}'.")
endfunction()
