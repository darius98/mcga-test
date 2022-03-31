#include "mcga/test.hpp"

#include <thread>
#include <unistd.h>

static std::thread::id outside_test_thread_id = std::this_thread::get_id();
static pid_t outside_test_proc_id = getpid();

static std::thread::id in_test_thread_id;
static pid_t in_test_proc_id;

void expect_inside_test() {
    mcga::test::expect(in_test_proc_id == getpid());
    mcga::test::expect(in_test_thread_id == std::this_thread::get_id());

    mcga::test::cleanup([] {
        mcga::test::expect(in_test_proc_id == getpid());
        mcga::test::expect(in_test_thread_id == std::this_thread::get_id());
    });
}

void expect_outside_test_ids() {
    mcga::test::expect(outside_test_proc_id == getpid());
    mcga::test::expect(outside_test_thread_id == std::this_thread::get_id());
}

TEST_CASE("test-case") {
    expect_outside_test_ids();

    setUp([] {
        in_test_thread_id = std::this_thread::get_id();
        in_test_proc_id = getpid();
    });

    setUp(expect_inside_test);
    setUp(expect_inside_test);
    setUp(expect_inside_test);
    tearDown(expect_inside_test);
    tearDown(expect_inside_test);
    tearDown(expect_inside_test);

    group("group", [] {
        expect_outside_test_ids();

        setUp(expect_inside_test);
        setUp(expect_inside_test);
        setUp(expect_inside_test);
        tearDown(expect_inside_test);
        tearDown(expect_inside_test);
        tearDown(expect_inside_test);

        test(expect_inside_test);
    });

    test(expect_inside_test);
}

struct TestFixture {
    TestFixture() {
        in_test_thread_id = std::this_thread::get_id();
        in_test_proc_id = getpid();
    }

    ~TestFixture() noexcept(false) {
        expect_inside_test();
    }

    void setUp() {
        expect_inside_test();
    }

    void SetUp() {
        expect_inside_test();
    }

    void setup() {
        expect_inside_test();
    }

    void set_up() {
        expect_inside_test();
    }

    void tearDown() {
        expect_inside_test();
    }

    void TearDown() {
        expect_inside_test();
    }

    void teardown() {
        expect_inside_test();
    }

    void tear_down() {
        expect_inside_test();
    }
};

TEST_F(TestFixture, "test-fixtures-api-with-fixture") {
    expect_inside_test();
}
