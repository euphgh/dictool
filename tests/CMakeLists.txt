# if local have gtest, use local gtest
find_package(GoogleTest QUIET)

# else fetch from github
if (NOT GTest_FOUND)
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

add_executable(aglorithm_test
  src/aglorithm/reverse.cc
  src/aglorithm/other.cc
)

target_link_libraries(multidict_test
    PRIVATE
        dictool
        GTest::gtest_main
        GTest::gmock
)

target_link_libraries(aglorithm_test
    PRIVATE
        dictool
        GTest::gtest_main
        GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(multidict_test)
gtest_discover_tests(aglorithm_test)

