#pragma once
#include <string>
#include <windows.h>
#include "employee.h"

class EmployeeRepository {
public:
    EmployeeRepository();
    ~EmployeeRepository();
    bool open(const std::string& path, bool create);
    bool initFromInput();
    void printAll() const;
    uint32_t count() const;
    bool readByIndex(uint32_t index, employee& out) const;
    bool writeByIndex(uint32_t index, const employee& in);
    int findById(int id) const;
private:
    HANDLE hFile_;
    uint32_t recordCount_;
};
