cmake_minimum_required(VERSION 3.20)
project(dictool VERSION 0.1.0 LANGUAGES CXX)

option(dictool_BUILD_TESTS "Build tests" ON)

add_library(dictool INTERFACE)
target_compile_features(dictool INTERFACE cxx_std_17)
target_include_directories(dictool INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ---------- Install (headers + CMake package) ----------
include(GNUInstallDirs)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS dictool
    EXPORT dictoolTargets
)

install(EXPORT dictoolTargets
    NAMESPACE dictool::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dictool
    FILE dictoolTargets.cmake
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/dictoolConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dictoolConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/dictoolConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dictool
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/dictoolConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/dictoolConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dictool
)

# ---------- Tests ----------
if(dictool_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()