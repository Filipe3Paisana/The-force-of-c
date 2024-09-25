#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct Queue_* Queue;

/**
 * @brief Create a new queue.
 * 
 * @return Queue The new queue.
 */
Queue queue_create();

/**
 * @brief Destroy the queue and free the elements.
 * 
 * @param queue The queue to destroy.
 * @param free_element The function to free the elements.
 */
void queue_destroy(Queue queue, void (*free_element)(void*));

/**
 * @brief Check if the queue is empty.
 * 
 * @param queue The queue to check.
 * @return true The queue is empty.
 * @return false The queue is not empty.
 */
bool queue_is_empty(Queue queue);

/**
 * @brief Check if the queue is full.
 * 
 * @param queue The queue to check.
 * @return true The queue is full.
 * @return false The queue is not full.
 */
bool is_full(Queue queue);

/**
 * @brief Get the size of the queue.
 * 
 * @param queue The queue to check.
 * @return int The size of the queue.
 */
void queue_enqueue(Queue queue, void* element);

/**
 * @brief Get the first element of the queue.
 * 
 * @param queue The queue to check.
 * @return void* The first element of the queue.
 */
void* queue_dequeue(Queue queue);

/**
 * @brief Get the first element of the queue without removing it.
 * 
 * @param queue The queue to check.
 * @return void* The first element of the queue.
 */
void* queue_peek(Queue queue);

#endif