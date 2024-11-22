enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(SOURS ${CMAKE_CURRENT_BINARY_DIR}/../src)

add_executable(run_tests tests.cpp)

target_link_libraries(run_tests ${GTEST_LIBRARIES} pthread)