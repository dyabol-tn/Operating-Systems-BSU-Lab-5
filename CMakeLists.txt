cmake_minimum_required(VERSION 3.20)
project(OS_Laboratory_Work_5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")

add_library(domain STATIC
    employee.cpp
    constants.h
    fileio.cpp
    pipe.cpp
    sync.cpp
    protocol.cpp
)

add_executable(server server.cpp)
add_executable(client client.cpp)
add_executable(test test.cpp)

target_link_libraries(server PRIVATE domain)
target_link_libraries(client PRIVATE domain)

if(MSVC)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(test PRIVATE gtest_main domain)

enable_testing()
add_test(NAME unit_tests COMMAND test)

set_target_properties(server client test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)