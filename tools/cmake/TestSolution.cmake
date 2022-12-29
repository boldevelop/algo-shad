function(add_exec NAME)
    set(MULTI_VALUE_ARGS PRIVATE_TESTS SOLUTION_SRCS)
    cmake_parse_arguments(SHAD_LIBRARY "" "" "${MULTI_VALUE_ARGS}" ${ARGN})

    add_executable(${NAME}
        ${SHAD_LIBRARY_UNPARSED_ARGUMENTS}
        ${SHAD_LIBRARY_SOLUTION_SRCS}
        ${SHAD_LIBRARY_PRIVATE_TESTS})

    message(TARGET ${TARGET} ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

function(add_catch TARGET)
    add_exec(${TARGET} ${ARGN})

    target_link_libraries(${TARGET} contrib_catch_main)
endfunction()
