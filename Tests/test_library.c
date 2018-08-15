#include "test_library.h"

void given_nothing() {}

void check_is_not_null(void * pointer) {
    if (pointer == NULL) {
        failed("Expected: a not null pointer.\nReceived: a null pointer.");
    } else {
        passed();
    }
}

void check_strings_equal(char * s1, char * s2) {
    if (strcmp(s1, s2) != 0) {
        failed("Expected: two equal strings.\nReceived: two different strings.");
    } else {
        passed();
    }
}

void then_success() {
    passed();
}

void passed() {
    printf("\tPassed!\n");
}

void failed(char * error) {
    printf("\tFailed\n");
    fprintf(stderr, "%s", error);
}

void not_implemented() {
    fflush(stdout);
    fprintf(stderr, "Not implemented");
    exit(1);
}
