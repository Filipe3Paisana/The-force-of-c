#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct Stack_* Stack;

/**
 * @brief Create a stack object.
 * 
 * @return Stack The stack created.
 */
Stack stack_create();

/**
 * @brief Destroy the stack object.
 * 
 * @param stack The stack to destroy.
 * @param free_element A function that frees the elements of the stack.
 */
void stack_destroy(Stack stack, void (*free_element)(void*));

/**
 * @brief Check if the stack is empty.
 * 
 * @param stack The stack to check.
 * @return true If the stack is empty.
 * @return false If the stack is not empty.
 */
bool stack_is_empty(Stack stack);

/**
 * @brief Check if the stack is full.
 * 
 * @param stack The stack to check.
 * @return true If the stack is full.
 * @return false If the stack is not full.
 */
bool stack_is_full(Stack stack);

/**
 * @brief Push an element to the stack.
 * 
 * @param stack The stack to push the element.
 * @param element The element to push.
 */
void stack_push(Stack stack, void* element);

/**
 * @brief Pop an element from the stack.
 * 
 * @param stack The stack to pop the element.
 * @return void* The element popped.
 */
void* stack_pop(Stack stack);

/**
 * @brief Get the top element of the stack.
 * 
 * @param stack The stack to get the top element.
 * @return void* The top element.
 */
void* stack_top(Stack stack);

#endif