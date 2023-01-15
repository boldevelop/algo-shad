add_catch(test_contest3_d test.cpp)
target_compile_definitions(test_contest3_d PUBLIC -DLOCAL)

add_executable(contest3_d answer.cpp)
