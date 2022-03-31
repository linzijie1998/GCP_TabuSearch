cmake_minimum_required(VERSION 3.5)

project(gcp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
        src/CTabuSearch.cpp
        src/main.cpp
)

add_executable(gcp ${SOURCES})

target_include_directories(gcp
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)