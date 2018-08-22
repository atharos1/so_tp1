#include "test_master.h"

int fd[2];
int ret_pipe;

int master_test_run() {
    printf("Testing pipe creation:");
    when_creating_pipe();
    then_pipe_is_created();

    printf("Testing item addition:");
    when_adding_item_to_pipe();
    then_pipe_is_not_empty();
    return 0;
}

void when_creating_pipe() {
    ret_pipe = pipe(fd);
}

void then_pipe_is_created() {
    if (ret_pipe != 0) {
        failed("Expected: pipe return equals 0.\nReceived: pipe return not 0.");
    } else {
        passed();
    }
}

void when_adding_item_to_pipe() {
    char * msg = "test";
    pipe_write(fd[WRITE], msg);
}

void then_pipe_is_not_empty() {
    char str[20];
    pipe_read(fd[READ], str);
    if (str == NULL) {
        failed("Expected: non=null buffer.\nReceived: null buffer.");
    } else {
        passed();
    }
}