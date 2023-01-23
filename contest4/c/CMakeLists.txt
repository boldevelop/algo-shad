add_catch(test_contest4_c test.cpp)
target_compile_definitions(test_contest4_c PUBLIC -DLOCAL)

add_executable(contest4_c answer.cpp)
