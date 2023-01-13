add_catch(test_contest3_b test.cpp)
target_compile_definitions(test_contest3_b PUBLIC -DLOCAL)

add_executable(contest3_b answer.cpp)
