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
    const char* name = "fixture1";

    void setUp() {
        printf("%s-setUp\n", name);
    }

    void tearDown() {
        printf("%s-tearDown\n", name);
    }
};

TEST_F(Fixture1, "fixture1-passing-test") {
    printf("%s-passing-test-body\n", this->name);
}

TEST_F(Fixture1, "fixture1-failing-test") {
    printf("%s-failing-test-body\n", name);
    mcga::test::fail("failure in fixture1");
}

// Pascal-case methods should also work

struct Fixture2 {
    const char* name = "fixture2";

    void SetUp() {
        printf("%s-setUp\n", name);
    }

    void TearDown() {
        printf("%s-tearDown\n", name);
    }
};

TEST_F(Fixture2, "fixture2-passing-test") {
    printf("%s-passing-test-body\n", this->name);
}

TEST_F(Fixture2, "fixture2-failing-test") {
    printf("%s-failing-test-body\n", name);
    mcga::test::fail("failure in fixture2");
}

// Mix-and-match casing should work

struct Fixture3 {
    const char* name = "fixture3";

    void set_up() {
        printf("%s-setUp\n", name);
    }

    void teardown() {
        printf("%s-tearDown\n", name);
    }
};

TEST_F(Fixture3, "fixture3-passing-test") {
    printf("%s-passing-test-body\n", this->name);
}

TEST_F(Fixture3, "fixture3-failing-test") {
    printf("%s-failing-test-body\n", name);
    mcga::test::fail("failure in fixture3");
}

struct Fixture4 {
    const char* name = "fixture4";

    void setup() {
        printf("%s-setUp\n", name);
    }

    void tear_down() {
        printf("%s-tearDown\n", name);
    }
};

TEST_F(Fixture4, "fixture4-passing-test") {
    printf("%s-passing-test-body\n", this->name);
}

TEST_F(Fixture4, "fixture4-failing-test") {
    printf("%s-failing-test-body\n", name);
    mcga::test::fail("failure in fixture4");
}
