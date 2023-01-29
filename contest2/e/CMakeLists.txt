add_catch(test_contest2_e test.cpp)
target_compile_definitions(test_contest2_e PUBLIC -DLOCAL)

add_executable(contest2_e answer.cpp)
