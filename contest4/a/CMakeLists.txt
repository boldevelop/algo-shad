add_catch(test_contest4_a test.cpp)
target_compile_definitions(test_contest4_a PUBLIC -DLOCAL)

add_executable(contest4_a answer.cpp)
