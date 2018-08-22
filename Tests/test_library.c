#include "test_library.h"

void check_is_not_null(void * pointer) {
    if (pointer == NULL) {
        failed("Expected: a not null pointer.\nReceived: a null pointer.");
    } else {
        passed();
    }
}

void passed() {
    printf("\tPassed!\n");
}

void failed(char * error) {
    printf("\tFailed\n");
    fprintf(stderr, "%s", error);
}
