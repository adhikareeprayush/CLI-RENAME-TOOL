cmake_minimum_required(VERSION 3.10)
project(cli-toolkit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rename-tool
        src/main.cpp
        src/rename_tool.cpp
)