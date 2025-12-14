#pragma once
#include <cstdint>

namespace constants {
    inline constexpr const char* PIPE_NAME = R"(\\.\pipe\employee_pipe)";
    inline constexpr uint32_t PIPE_OUT_BUF = 4096;
    inline constexpr uint32_t PIPE_IN_BUF = 4096;
    inline constexpr const char* SEM_WRITE_NAME = "emp_write_sem";
    inline constexpr const char* READERS_MUTEX_NAME = "emp_readers_mutex";
    inline constexpr size_t NAME_MAX_LEN = 10;
}