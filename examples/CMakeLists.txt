cmake_minimum_required(VERSION 3.20)
project(BuriedExample)

# 添加链接静态库的可执行程序
add_executable(buried_example buried_example.cpp)
target_link_libraries(buried_example Buried_static)

# 添加链接动态库的可执行程序
add_executable(buried_example_shared buried_example.cpp)
target_link_libraries(buried_example_shared Buried_shared)