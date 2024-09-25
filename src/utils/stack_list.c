#include <stdlib.h>
#include "list.h"
#include "stack.h"

struct Stack_ {
    List list;
};

Stack stack_create() {
    Stack stack = malloc(sizeof(Stack));
    stack->list = list_create();
    return stack;
}

void stack_destroy(Stack stack, void (*free_element)(void*)) {
    list_destroy(stack->list, free_element);
    free(stack);
}

bool stack_is_empty(Stack stack) {
    return list_is_empty(stack->list);
}

bool stack_is_full(Stack stack) {
    return false;
}

void stack_push(Stack stack, void* element) {
    list_insert_first(stack->list, element);
}

void* stack_pop(Stack stack) {
    return list_remove_first(stack->list);
}

void* stack_top(Stack stack) {
    return list_get_first(stack->list);
}

