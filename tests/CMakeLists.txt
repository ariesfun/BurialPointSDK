cmake_minimum_required(VERSION 3.20)
project(BuriedTest)

add_definitions(-D_WIN32_WINNT=0x0601)
include_directories(. ../src/module/)

set(TEST_SRC 
    test.cpp
    test_db.cpp
    test_http.cpp
    test_crypt.cpp)

add_executable(buried_test ${TEST_SRC})
target_link_libraries(buried_test Buried_static gtest)