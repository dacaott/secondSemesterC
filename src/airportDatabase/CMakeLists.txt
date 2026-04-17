
cmake_minimum_required(VERSION 3.25)
project(2_sem_homework C)

add_executable(airport_manager main.c cmdUtils.c tree.c)
add_executable(test_avl test.c tree.c)

target_include_directories(airport_manager PRIVATE src)
target_include_directories(test_avl PRIVATE src)
