#pragma once
#include <cstdint>
#include "employee.h"

enum class OpCode : uint8_t { Read = 1, Write = 2, Quit = 3 };

class NamedPipeServer;
class NamedPipeClient;

bool writeOp(NamedPipeServer& pipe, OpCode op);
bool writeOp(NamedPipeClient& pipe, OpCode op);
bool readOp(NamedPipeServer& pipe, OpCode& op);
bool readOp(NamedPipeClient& pipe, OpCode& op);

bool writeInt(NamedPipeServer& pipe, int v);
bool writeInt(NamedPipeClient& pipe, int v);
bool readInt(NamedPipeServer& pipe, int& v);
bool readInt(NamedPipeClient& pipe, int& v);

bool writeByte(NamedPipeServer& pipe, uint8_t b);
bool writeByte(NamedPipeClient& pipe, uint8_t b);
bool readByte(NamedPipeServer& pipe, uint8_t& b);
bool readByte(NamedPipeClient& pipe, uint8_t& b);

bool writeEmployee(NamedPipeServer& pipe, const employee& e);
bool writeEmployee(NamedPipeClient& pipe, const employee& e);
bool readEmployee(NamedPipeServer& pipe, employee& e);
bool readEmployee(NamedPipeClient& pipe, employee& e);
