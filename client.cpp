#include <iostream>
#include <string>
#include <cstring>
#include "constants.h"
#include "pipe.h"
#include "protocol.h"
#include "employee.h"

int main() {
    NamedPipeClient client;
    if (!client.connect(constants::PIPE_NAME)) return 1;
    while (true) {
        std::cout << "Choose: r=read, w=write, q=quit: ";
        char c;
        std::cin >> c;
        if (c == 'q') {
            if (!writeOp(client, OpCode::Quit)) break;
            break;
        }
        else if (c == 'r') {
            std::cout << "Enter ID: ";
            int id;
            std::cin >> id;
            if (!writeOp(client, OpCode::Read)) break;
            if (!writeInt(client, id)) break;
            uint8_t ok = 0;
            if (!readByte(client, ok)) break;
            if (!ok) {
                std::cout << "Record not found" << std::endl;
                continue;
            }
            employee e{};
            if (!readEmployee(client, e)) break;
            std::cout << e.num << " " << e.name << " " << e.hours << std::endl;
        }
        else if (c == 'w') {
            std::cout << "Enter ID: ";
            int id;
            std::cin >> id;
            if (!writeOp(client, OpCode::Write)) break;
            if (!writeInt(client, id)) break;
            uint8_t ok = 0;
            if (!readByte(client, ok)) break;
            if (!ok) {
                std::cout << "Record not found" << std::endl;
                continue;
            }
            employee current{};
            if (!readEmployee(client, current)) break;
            std::cout << current.num << " " << current.name << " " << current.hours << std::endl;
            employee updated{};
            updated.num = current.num;
            std::string newName;
            double newHours;
            std::cout << "New Name Hours: ";
            std::cin >> newName >> newHours;
            std::memset(updated.name, 0, sizeof(updated.name));
            std::strncpy(updated.name, newName.c_str(), sizeof(updated.name) - 1);
            updated.hours = newHours;
            if (!writeEmployee(client, updated)) break;
            uint8_t ack = 0;
            if (!readByte(client, ack)) break;
            if (ack) std::cout << "Updated" << std::endl;
        }
    }
    return 0;
}