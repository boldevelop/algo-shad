add_catch(test_contest3_c test.cpp)
target_compile_definitions(test_contest3_c PUBLIC -DLOCAL)

add_executable(contest3_c answer.cpp)
