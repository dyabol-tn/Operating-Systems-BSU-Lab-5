#include "pipe.h"

NamedPipeServer::NamedPipeServer() : hPipe_(INVALID_HANDLE_VALUE) {}
NamedPipeServer::~NamedPipeServer() {
    if (hPipe_ != INVALID_HANDLE_VALUE) {
        DisconnectNamedPipe(hPipe_);
        CloseHandle(hPipe_);
    }
}
bool NamedPipeServer::create(const std::string& name, uint32_t outBuf, uint32_t inBuf) {
    hPipe_ = CreateNamedPipeA(name.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, outBuf, inBuf, INFINITE, NULL);
    return hPipe_ != INVALID_HANDLE_VALUE;
}
bool NamedPipeServer::accept() { return ConnectNamedPipe(hPipe_, NULL) != 0; }
bool NamedPipeServer::read(void* buf, DWORD n) { DWORD rd = 0; return ReadFile(hPipe_, buf, n, &rd, NULL) && rd == n; }
bool NamedPipeServer::write(const void* buf, DWORD n) { DWORD wr = 0; return WriteFile(hPipe_, buf, n, &wr, NULL) && wr == n; }

NamedPipeClient::NamedPipeClient() : hPipe_(INVALID_HANDLE_VALUE) {}
NamedPipeClient::~NamedPipeClient() { if (hPipe_ != INVALID_HANDLE_VALUE) CloseHandle(hPipe_); }
bool NamedPipeClient::connect(const std::string& name) {
    if (!WaitNamedPipeA(name.c_str(), NMPWAIT_WAIT_FOREVER)) return false;
    hPipe_ = CreateFileA(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    return hPipe_ != INVALID_HANDLE_VALUE;
}
bool NamedPipeClient::read(void* buf, DWORD n) { DWORD rd = 0; return ReadFile(hPipe_, buf, n, &rd, NULL) && rd == n; }
bool NamedPipeClient::write(const void* buf, DWORD n) { DWORD wr = 0; return WriteFile(hPipe_, buf, n, &wr, NULL) && wr == n; }
