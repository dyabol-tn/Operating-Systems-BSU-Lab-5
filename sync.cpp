#include "sync.h"

ReadersWriters::ReadersWriters() : hWriteSem_(NULL), hReadersMutex_(NULL), readersCount_(0) {}
ReadersWriters::~ReadersWriters() { if (hWriteSem_) CloseHandle(hWriteSem_); if (hReadersMutex_) CloseHandle(hReadersMutex_); }
bool ReadersWriters::init(const std::string& writeSemName, const std::string& readersMutexName) {
    hWriteSem_ = CreateSemaphoreA(NULL, 1, 1, writeSemName.c_str());
    hReadersMutex_ = CreateMutexA(NULL, FALSE, readersMutexName.c_str());
    return hWriteSem_ != NULL && hReadersMutex_ != NULL;
}
bool ReadersWriters::acquireRead() {
    if (WaitForSingleObject(hReadersMutex_, INFINITE) != WAIT_OBJECT_0) return false;
    readersCount_++;
    if (readersCount_ == 1) {
        if (WaitForSingleObject(hWriteSem_, INFINITE) != WAIT_OBJECT_0) {
            ReleaseMutex(hReadersMutex_);
            return false;
        }
    }
    ReleaseMutex(hReadersMutex_);
    return true;
}
void ReadersWriters::releaseRead() {
    WaitForSingleObject(hReadersMutex_, INFINITE);
    readersCount_--;
    if (readersCount_ == 0) ReleaseSemaphore(hWriteSem_, 1, NULL);
    ReleaseMutex(hReadersMutex_);
}
bool ReadersWriters::acquireWrite() { return WaitForSingleObject(hWriteSem_, INFINITE) == WAIT_OBJECT_0; }
void ReadersWriters::releaseWrite() { ReleaseSemaphore(hWriteSem_, 1, NULL); }
