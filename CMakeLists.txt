cmake_minimum_required(VERSION 3.0.0)
project(cchecker VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_executable(cchecker main.cpp lexer.cpp lexer.hpp token.hpp parser.hpp parser.cpp node.hpp node.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
