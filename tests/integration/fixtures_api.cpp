#include "mcga/test.hpp"

#include <cstdio>

TEST("passing-test") {
    printf("passing-test-body\n");
}

TEST("failing-test") {
    printf("failing-test-body\n");
    mcga::test::fail("failure in global");
}

struct Fixture1 {
    void setUp() {
        printf("fixture1-setUp\n");
    }

    void tearDown() {
        printf("fixture1-tearDown\n");
    }
};

TEST_F(Fixture1, "fixture1-passing-test") {
    printf("fixture1-passing-test-body\n");
}

TEST_F(Fixture1, "fixture1-failing-test") {
    printf("fixture1-failing-test-body\n");
    mcga::test::fail("failure in fixture1");
}

// Pascal-case methods should also work

struct Fixture2 {
    void SetUp() {
        printf("fixture2-setUp\n");
    }

    void TearDown() {
        printf("fixture2-tearDown\n");
    }
};

TEST_F(Fixture2, "fixture2-passing-test") {
    printf("fixture2-passing-test-body\n");
}

TEST_F(Fixture2, "fixture2-failing-test") {
    printf("fixture2-failing-test-body\n");
    mcga::test::fail("failure in fixture2");
}

// Mix-and-match casing should work

struct Fixture3 {
    void set_up() {
        printf("fixture3-setUp\n");
    }

    void teardown() {
        printf("fixture3-tearDown\n");
    }
};

TEST_F(Fixture3, "fixture3-passing-test") {
    printf("fixture3-passing-test-body\n");
}

TEST_F(Fixture3, "fixture3-failing-test") {
    printf("fixture3-failing-test-body\n");
    mcga::test::fail("failure in fixture3");
}

struct Fixture4 {
    void setup() {
        printf("fixture4-setUp\n");
    }

    void tear_down() {
        printf("fixture4-tearDown\n");
    }
};

TEST_F(Fixture4, "fixture4-passing-test") {
    printf("fixture4-passing-test-body\n");
}

TEST_F(Fixture4, "fixture4-failing-test") {
    printf("fixture4-failing-test-body\n");
    mcga::test::fail("failure in fixture4");
}
