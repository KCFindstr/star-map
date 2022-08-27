cmake_minimum_required(VERSION 3.14)
project(star_map VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add sources
set(SOURCES
  src/expr_lib.cc
  src/graph.cc
  src/node_ref.cc
)

add_library(star_map_lib STATIC ${SOURCES})
target_include_directories(star_map_lib PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Add tests
set(TESTS
  test/graph_test.cc
)
enable_testing()

add_executable(
  star_map_test
  ${TESTS}
)
target_link_libraries(
  star_map_test
  GTest::gtest_main
  star_map_lib
)

include(GoogleTest)
target_include_directories(star_map_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
gtest_discover_tests(star_map_test)

# Set compiler flags
if(MSVC)
  target_compile_options(star_map_lib PRIVATE /std:c++20 /W3 /D_STAR_MAP_DEBUG)
  target_compile_options(star_map_test PRIVATE /std:c++20 /W3)
else()
  target_compile_options(star_map_lib PRIVATE -std=c++2a -Wall -Wextra -pedantic -D_STAR_MAP_DEBUG)
  target_compile_options(star_map_test PRIVATE -std=c++2a -Wall -Wextra -pedantic)
endif()
