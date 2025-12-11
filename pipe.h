#pragma once
#include <windows.h>
#include <string>

class NamedPipeServer {
public:
    NamedPipeServer();
    ~NamedPipeServer();
    bool create(const std::string& name, uint32_t outBuf, uint32_t inBuf);
    bool accept();
    bool read(void* buf, DWORD n);
    bool write(const void* buf, DWORD n);
private:
    HANDLE hPipe_;
};

class NamedPipeClient {
public:
    NamedPipeClient();
    ~NamedPipeClient();
    bool connect(const std::string& name);
    bool read(void* buf, DWORD n);
    bool write(const void* buf, DWORD n);
private:
    HANDLE hPipe_;
};