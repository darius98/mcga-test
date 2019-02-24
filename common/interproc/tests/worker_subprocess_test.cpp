#include <iostream>

#include <kktest.hpp>

#include "common/interproc/src/worker_subprocess.hpp"

using namespace kktest;
using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;

void interprocWorkerSubprocessTest() {
    group("Send a message, then die", [] {
        WorkerSubprocess* proc = nullptr;

        setUp([&] {
            proc = new WorkerSubprocess(50_ms, [](PipeWriter* writer) {
                writer->sendMessage(2, 0, 1, 9);
            });
            sleepForDuration(50_ms);
        });

        tearDown([&] {
            delete proc;
            proc = nullptr;
        });

        test("isFinished() == true", [&] {
            expect(proc->isFinished());
        });

        test("kill() == ALREADY_DEAD", [&] {
            expect(proc->kill() == Subprocess::ALREADY_DEAD);
        });

        test("isExited() == true", [&] {
            expect(proc->isExited());
        });

        test("getReturnCode() == 0", [&] {
            expect(proc->getReturnCode() == 0);
        });

        test("isSignaled() == false", [&] {
            expect(!proc->isSignaled());
        });

        test("getOutput() is empty", [&] {
            expect(proc->getOutput().empty());
        });

        test("getFinishStatus() == ZERO_EXIT", [&] {
            expect(proc->getFinishStatus() == Subprocess::ZERO_EXIT);
        });

        test("getNextMessage(32) is the sent message", [&] {
            auto message = proc->getNextMessage(32);
            expect(!message.isInvalid(), "Message is invalid.");
            int a, b, c, d;
            message >> a >> b >> c >> d;
            expect(a == 2 && b == 0 && c == 1 && d == 9,
                   "Message content is invalid.");
        });
    });

    group("Do nothing and die", [] {
        WorkerSubprocess* proc = nullptr;

        tearDown([&] {
            delete proc;
            proc = nullptr;
        });

        test("getNextMessage(32) is invalid", [&] {
            proc = new WorkerSubprocess(50_ms, [](PipeWriter*) {});
            sleepForDuration(50_ms);
            expect(proc->getNextMessage(32).isInvalid());
        });
    });

    group("Write some output and die", [] {
        WorkerSubprocess* proc = nullptr;

        tearDown([&] {
            delete proc;
            proc = nullptr;
        });

        test("getOutput() returns the written output", [&] {
            proc = new WorkerSubprocess(50_ms, [](PipeWriter*) {
                cout << "Hello, World!";
                cout.flush();
            });
            sleepForDuration(50_ms);
            expect(proc->getOutput() == "Hello, World!");
        });
    });

    group("Timeout", [] {
        WorkerSubprocess* proc = nullptr;

        tearDown([&] {
            delete proc;
            proc = nullptr;
        });

        test("getFinishStatus()==TIMEOUT", [&] {
            proc = new WorkerSubprocess(50_ms, [](PipeWriter*) {
                spinForDuration(200_ms);
            });
            sleepForDuration(100_ms);
            expect(proc->getFinishStatus() == Subprocess::TIMEOUT);
        });
    });
}