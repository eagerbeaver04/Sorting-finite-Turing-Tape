add_library(interface STATIC
    delays.hpp
    i_tape.hpp
)
set_target_properties(interface PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(interface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})