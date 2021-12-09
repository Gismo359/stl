cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(Example LANGUAGES CXX)

make_executable(Example)

target_link_libraries(Example Base)
