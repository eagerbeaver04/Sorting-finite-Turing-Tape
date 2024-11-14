add_library(tape STATIC
    file_tape.hpp
    file_tape.cpp
    tape_sorter.hpp
)
set_target_properties(tape PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(tape PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})