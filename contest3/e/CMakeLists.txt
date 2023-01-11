add_catch(test_contest3_e test.cpp)

add_executable(contest3_e answer.cpp)
target_compile_definitions(contest3_e PUBLIC -DANSWER)
