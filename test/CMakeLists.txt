cmake_minimum_required(VERSION 3.6.0)
project(test VERSION 0.0.1 LANGUAGES CXX)


# Make test executable
add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} doctest)
# target_include_directories(${PROJECT_NAME} PUBLIC ../3rdparty/doctest)
