#include "protocol.h"
#include "pipe.h"

bool writeOp(NamedPipeServer& pipe, OpCode op) { uint8_t o = static_cast<uint8_t>(op); return pipe.write(&o, 1); }
bool writeOp(NamedPipeClient& pipe, OpCode op) { uint8_t o = static_cast<uint8_t>(op); return pipe.write(&o, 1); }
bool readOp(NamedPipeServer& pipe, OpCode& op) { uint8_t o = 0; if (!pipe.read(&o, 1)) return false; op = static_cast<OpCode>(o); return true; }
bool readOp(NamedPipeClient& pipe, OpCode& op) { uint8_t o = 0; if (!pipe.read(&o, 1)) return false; op = static_cast<OpCode>(o); return true; }

bool writeInt(NamedPipeServer& pipe, int v) { return pipe.write(&v, sizeof(v)); }
bool writeInt(NamedPipeClient& pipe, int v) { return pipe.write(&v, sizeof(v)); }
bool readInt(NamedPipeServer& pipe, int& v) { return pipe.read(&v, sizeof(v)); }
bool readInt(NamedPipeClient& pipe, int& v) { return pipe.read(&v, sizeof(v)); }

bool writeByte(NamedPipeServer& pipe, uint8_t b) { return pipe.write(&b, 1); }
bool writeByte(NamedPipeClient& pipe, uint8_t b) { return pipe.write(&b, 1); }
bool readByte(NamedPipeServer& pipe, uint8_t& b) { return pipe.read(&b, 1); }
bool readByte(NamedPipeClient& pipe, uint8_t& b) { return pipe.read(&b, 1); }

bool writeEmployee(NamedPipeServer& pipe, const employee& e) { return pipe.write(&e, sizeof(e)); }
bool writeEmployee(NamedPipeClient& pipe, const employee& e) { return pipe.write(&e, sizeof(e)); }
bool readEmployee(NamedPipeServer& pipe, employee& e) { return pipe.read(&e, sizeof(e)); }
bool readEmployee(NamedPipeClient& pipe, employee& e) { return pipe.read(&e, sizeof(e)); }
