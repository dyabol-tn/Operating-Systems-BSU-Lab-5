#include <iostream>
#include <string>
#include "constants.h"
#include "fileio.h"
#include "pipe.h"
#include "sync.h"
#include "protocol.h"

int main() {
    std::string path;
    std::cout << "Enter binary filename: ";
    std::cin >> path;

    EmployeeRepository repo;
    if (!repo.open(path, true)) return 1;
    if (!repo.initFromInput()) return 1;
    repo.printAll();

    ReadersWriters rw;
    if (!rw.init(constants::SEM_WRITE_NAME, constants::READERS_MUTEX_NAME)) return 1;

    NamedPipeServer server;
    if (!server.create(constants::PIPE_NAME, constants::PIPE_OUT_BUF, constants::PIPE_IN_BUF)) return 1;
    std::cout << "Waiting client..." << std::endl;
    if (!server.accept()) return 1;
    std::cout << "Client connected" << std::endl;

    while (true) {
        OpCode op;
        if (!readOp(server, op)) break;
        if (op == OpCode::Quit) break;
        int id = 0;
        if (!readInt(server, id)) break;
        if (op == OpCode::Read) {
            if (!rw.acquireRead()) break;
            int idx = repo.findById(id);
            uint8_t ok = idx >= 0 ? 1 : 0;
            if (!writeByte(server, ok)) { rw.releaseRead(); break; }
            if (ok) {
                employee e{};
                repo.readByIndex(static_cast<uint32_t>(idx), e);
                if (!writeEmployee(server, e)) { rw.releaseRead(); break; }
            }
            rw.releaseRead();
        }
        else if (op == OpCode::Write) {
            if (!rw.acquireWrite()) break;
            int idx = repo.findById(id);
            uint8_t ok = idx >= 0 ? 1 : 0;
            if (!writeByte(server, ok)) { rw.releaseWrite(); break; }
            if (ok) {
                employee current{};
                repo.readByIndex(static_cast<uint32_t>(idx), current);
                if (!writeEmployee(server, current)) { rw.releaseWrite(); break; }
                employee updated{};
                if (!readEmployee(server, updated)) { rw.releaseWrite(); break; }
                repo.writeByIndex(static_cast<uint32_t>(idx), updated);
                uint8_t ack = 1;
                if (!writeByte(server, ack)) { rw.releaseWrite(); break; }
            }
            rw.releaseWrite();
        }
    }

    repo.printAll();
    return 0;
}