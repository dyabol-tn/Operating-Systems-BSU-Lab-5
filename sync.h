#pragma once
#include <windows.h>
#include <string>

class ReadersWriters {
public:
    ReadersWriters();
    ~ReadersWriters();
    bool init(const std::string& writeSemName, const std::string& readersMutexName);
    bool acquireRead();
    void releaseRead();
    bool acquireWrite();
    void releaseWrite();
private:
    HANDLE hWriteSem_;
    HANDLE hReadersMutex_;
    long readersCount_;
};
