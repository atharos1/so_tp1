#include "test_slave.h"

char test_path[20];
char buffer[60];

int slave_test_run() {
    printf("Test MD5 Hashing:");
    test_MD5_hashing();
    return 0;
}

void test_MD5_hashing() {
    given_existing_file();
    when_calculating_hash();
    then_return_value_is_not_null();
}

void given_existing_file() {
    sprintf(test_path, "pipemng.c");
}

void when_calculating_hash() {
    calculate_MD5(test_path, buffer);
}

void then_return_value_is_not_null() {
    check_is_not_null(buffer);
}