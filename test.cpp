#include <gtest/gtest.h>
#include <thread>
#include "employee.h"
#include "fileio.h"
#include "protocol.h"
#include "pipe.h"
#include "sync.h"

TEST(EmployeeRepositoryTest, WriteAndRead) {
    EmployeeRepository repo;
    ASSERT_TRUE(repo.open("test.bin", true));
    employee e{ 1,"Ivan",40.0 };
    ASSERT_TRUE(repo.writeByIndex(0, e));
    employee out{};
    ASSERT_TRUE(repo.readByIndex(0, out));
    EXPECT_EQ(out.num, 1);
    EXPECT_STREQ(out.name, "Ivan");
    EXPECT_DOUBLE_EQ(out.hours, 40.0);
}

TEST(EmployeeRepositoryTest, FindById) {
    EmployeeRepository repo;
    ASSERT_TRUE(repo.open("test2.bin", true));
    employee e1{ 1,"A",10.0 };
    employee e2{ 2,"B",20.0 };
    ASSERT_TRUE(repo.writeByIndex(0, e1));
    ASSERT_TRUE(repo.writeByIndex(1, e2));
    EXPECT_EQ(repo.findById(2), 1);
    EXPECT_EQ(repo.findById(3), -1);
}

TEST(ProtocolTest, SerializeDeserializeOpCode) {
    NamedPipeServer server;
    NamedPipeClient client;
    ASSERT_TRUE(server.create(R"(\\.\pipe\test_pipe1)", 4096, 4096));
    std::thread t([&]() { ASSERT_TRUE(client.connect(R"(\\.\pipe\test_pipe1)")); });
    ASSERT_TRUE(server.accept());
    t.join();
    ASSERT_TRUE(writeOp(server, OpCode::Read));
    OpCode op;
    ASSERT_TRUE(readOp(client, op));
    EXPECT_EQ(op, OpCode::Read);
}

TEST(ProtocolTest, IntRoundtrip) {
    NamedPipeServer server;
    NamedPipeClient client;
    ASSERT_TRUE(server.create(R"(\\.\pipe\test_pipe2)", 4096, 4096));
    std::thread t([&]() { ASSERT_TRUE(client.connect(R"(\\.\pipe\test_pipe2)")); });
    ASSERT_TRUE(server.accept());
    t.join();
    int v = 12345, r = 0;
    ASSERT_TRUE(writeInt(server, v));
    ASSERT_TRUE(readInt(client, r));
    EXPECT_EQ(r, v);
}

TEST(SyncTest, ReadersWritersBasic) {
    ReadersWriters rw;
    ASSERT_TRUE(rw.init("sem_write_test", "mutex_readers_test"));
    ASSERT_TRUE(rw.acquireRead());
    rw.releaseRead();
    ASSERT_TRUE(rw.acquireWrite());
    rw.releaseWrite();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}