# if local have gtest, use local gtest
find_package(GTest QUIET)

set(gtest_use_fetch FALSE)
set(MIN_GTEST_VERSION 1.14.0)
if (NOT TARGET GTest::gtest_main)
    message(STATUS "GTest not found locally.")
    set(gtest_use_fetch TRUE)
elseif (GTest_VERSION VERSION_LESS MIN_GTEST_VERSION)
    message(STATUS "GTest version ${GTest_VERSION} is too old (< ${MIN_GTEST_VERSION}), will fetch new version.")
    set(gtest_use_fetch TRUE)
endif()

# else fetch from github
if (NOT TARGET GTest::gtest_main)
    message(STATUS "GTest not found locally, fetching from GitHub...")

    FetchContent_Declare(
        googletest
        # Pin to a known-good release
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # On MSVC use /MD
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)
endif()


add_executable(multidict_test
  src/multidict/constructor.cc
  src/multidict/erase.cc
  src/multidict/insert.cc
  src/multidict/instance.cc
  src/multidict/iterator.cc
  src/multidict/misc.cc
  src/multidict/search.cc
)

target_link_libraries(multidict_test
    PRIVATE
        dictool
        GTest::gtest_main
        GTest::gmock
)

gtest_discover_tests(multidict_test)
