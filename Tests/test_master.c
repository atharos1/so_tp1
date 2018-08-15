#include "test_master.h"

int queue_id;
key_t queue_key = 12345;

int test_run() {
    printf("Testing queue creation:");
    when_creating_queue();
    then_queue_is_created();

    printf("Testing item addition:");
    when_adding_item_to_queue();
    then_queue_is_not_empty();
}

void when_creating_queue() {
    queue_id = queue_create(queue_key);
}

void then_queue_is_created() {
    if (queue_id < 0) {
        failed("Expected: queue_id greater than 0.\nReceived: queue_id lower than 0.");
    } else {
        passed();
    }
}

void when_adding_item_to_queue() {
    char ** msg = "test";
    post_files(1, 1, msg, 0);
}

void then_queue_is_not_empty() {
    struct message msg;
    queue_read(queue_id, &msg, SLAVE_QUEUE_ID, 0, 0);
    if (msg.text == NULL) {
        failed("Expected: non=null buffer.\nReceived: null buffer.");
    } else {
        passed();
    }
}