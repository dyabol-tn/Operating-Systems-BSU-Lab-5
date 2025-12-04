#include "fileio.h"
#include <iostream>
#include <cstring>

EmployeeRepository::EmployeeRepository() : hFile_(INVALID_HANDLE_VALUE), recordCount_(0) {}
EmployeeRepository::~EmployeeRepository() {
    if (hFile_ != INVALID_HANDLE_VALUE) CloseHandle(hFile_);
}
bool EmployeeRepository::open(const std::string& path, bool create) {
    DWORD disp = create ? CREATE_ALWAYS : OPEN_EXISTING;
    hFile_ = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, disp, FILE_ATTRIBUTE_NORMAL, NULL);
    return hFile_ != INVALID_HANDLE_VALUE;
}
bool EmployeeRepository::initFromInput() {
    std::cout << "Enter number of records: ";
    std::cin >> recordCount_;
    if (recordCount_ == 0) return false;
    for (uint32_t i = 0; i < recordCount_; ++i) {
        employee e{};
        std::string name;
        std::cout << "ID Name Hours: ";
        std::cin >> e.num >> name >> e.hours;
        std::memset(e.name, 0, sizeof(e.name));
        std::strncpy(e.name, name.c_str(), sizeof(e.name) - 1);
        if (!writeByIndex(i, e)) return false;
    }
    return true;
}
void EmployeeRepository::printAll() const {
    employee e{};
    for (uint32_t i = 0; i < recordCount_; ++i) {
        if (!readByIndex(i, e)) return;
        std::cout << e.num << " " << e.name << " " << e.hours << "\n";
    }
}
uint32_t EmployeeRepository::count() const { return recordCount_; }
bool EmployeeRepository::readByIndex(uint32_t index, employee& out) const {
    LARGE_INTEGER pos;
    pos.QuadPart = static_cast<LONGLONG>(index) * static_cast<LONGLONG>(sizeof(employee));
    if (!SetFilePointerEx(hFile_, pos, nullptr, FILE_BEGIN)) return false;
    DWORD rd = 0;
    if (!ReadFile(hFile_, &out, sizeof(employee), &rd, nullptr)) return false;
    return rd == sizeof(employee);
}
bool EmployeeRepository::writeByIndex(uint32_t index, const employee& in) {
    LARGE_INTEGER pos;
    pos.QuadPart = static_cast<LONGLONG>(index) * static_cast<LONGLONG>(sizeof(employee));
    if (!SetFilePointerEx(hFile_, pos, nullptr, FILE_BEGIN)) return false;
    DWORD wr = 0;
    if (!WriteFile(hFile_, &in, sizeof(employee), &wr, nullptr)) return false;
    return wr == sizeof(employee);
}
int EmployeeRepository::findById(int id) const {
    employee e{};
    for (uint32_t i = 0; i < recordCount_; ++i) {
        if (!readByIndex(i, e)) return -1;
        if (e.num == id) return static_cast<int>(i);
    }
    return -1;
}
