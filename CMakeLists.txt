cmake_minimum_required(VERSION 3.0)
project(raylib_template C)

set(CMAKE_C_STANDARD 99)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

add_subdirectory(libs/raylib)

add_executable(raylib_template src/main.c)
add_executable(Example1 src/susievector.c
                                src/example1.c)
add_executable(Exercise1 src/susievector.c
                                src/exercise1.c)
add_executable(Exercise2 src/susievector.c
                            src/susietrig.c
                                src/exercise2.c)
add_executable(Example2 src/susievector.c
                            src/susietrig.c
                                src/example2.c)
add_executable(Exercise3 src/susievector.c
                            src/susietrig.c
                                src/exercise3.c)
add_executable(Exercise4 src/susievector.c
                            src/susietrig.c
                                src/exercise4.c)
add_executable(Example4 src/susievector.c
                            src/susietrig.c
                                src/example4.c)
add_executable(Exercise5 src/susievector.c
                            src/susietrig.c
                                src/exercise5.c)
add_executable(Exercise6 src/susievector.c
                            src/susietrig.c
                                src/exercise6.c)
add_executable(Example5 src/susievector.c
                            src/susietrig.c
                                src/example5.c)

target_link_libraries(raylib_template PRIVATE raylib)
target_link_libraries(Example1 PRIVATE raylib)
target_link_libraries(Exercise1 PRIVATE raylib)
target_link_libraries(Exercise2 PRIVATE raylib)
target_link_libraries(Example2 PRIVATE raylib)
target_link_libraries(Exercise3 PRIVATE raylib)
target_link_libraries(Exercise4 PRIVATE raylib)
target_link_libraries(Example4 PRIVATE raylib)
target_link_libraries(Exercise5 PRIVATE raylib)
target_link_libraries(Exercise6 PRIVATE raylib)
target_link_libraries(Example5 PRIVATE raylib)