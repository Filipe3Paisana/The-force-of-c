#include <stdlib.h>
#include "queue.h"
#include "list.h"

struct Queue_ {
    List list;
};

Queue queue_create() {
    Queue queue = malloc(sizeof(Queue));
    queue->list = list_create();
    return queue;
}

void queue_destroy(Queue queue, void (*free_element)(void*)) {
    list_destroy(queue->list, free_element);
    free(queue);
}

bool queue_is_empty(Queue queue) {
    return list_is_empty(queue->list);
}

bool is_full(Queue queue) {
    return false;
}

void queue_enqueue(Queue queue, void* element) {
    list_insert_last(queue->list, element);
}

void* queue_dequeue(Queue queue) {
    return list_remove_first(queue->list);
}

void* queue_peek(Queue queue) {
    return list_get_first(queue->list);
}
