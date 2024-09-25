#include "list.h"
#include <stdlib.h>

typedef struct Node_* Node;

struct Node_ {
    Node next;
    void* element;
};

struct List_ {
    Node head;
    Node tail;
    Node iterator;
    int size;
};

void* free_node(Node node);

/**
 * @brief Creates a new list.
 *
 * @return List The new list.
 */
List list_create() {
    List list = malloc(sizeof(struct List_));
    list->head = NULL;
    list->tail = NULL;
    list->iterator = NULL;
    list->size = 0;
    return list;
}

/**
 * @brief Destroys a list.
 *
 * Frees all memory allocated for the list, and for all elements of the list.
 *
 * @param list The list to destroy.
 * @param free_element The function to free the elements of the list.
 */
void list_destroy(List list, void (*free_element)(void*)) {
    Node node = list->head;
    while (node != NULL) {
        Node next = node->next;
        if (free_element != NULL) {
            free_element(node->element);
        }
        free(node);
        node = next;
    }
    free(list);
}

/**
 * @brief Returns true iff the list contains no elements.
 *
 * @param list The linked list.
 * @return true iff the list contains no elements.
 */
bool list_is_empty(List list) {
    return list->size == 0;
}

/**
 * @brief Returns the number of elements in the list.
 *
 * @param list The linked list.
 * @return size_t The number of elements in the list.
 */
int list_size(List list) {
    return list->size;
}

/**
 * @brief Returns the first element of the list.
 *
 * @param list The linked list.
 * @return void* The first element of the list.
 */
void* list_get_first(List list) {
    if (list_is_empty(list)) {
        return NULL;
    }
    return list->head->element;
}

/**
 * @brief Returns the last element of the list.
 *
 * @param list The linked list.
 * @return void* The last element of the list.
 */
void* list_get_last(List list) {
    if (list_is_empty(list)) {
        return NULL;
    }
    return list->tail->element;
}

/**
 * @brief Returns the element at the specified position in the list.
 *
 * Range of valid positions: 0, ..., size()-1.
 *
 * @param list The linked list.
 * @param position The position of the element to return.
 * @return void* The element at the specified position in the list.
 */
void* list_get(List list, int position) {
    if (position >= list_size(list)) {
        return NULL;
    }
    if (position == list_size(list) - 1) {
        return list->tail->element;
    }
    if (position == 0) {
        return list->head->element;
    }
    int idx = 1;
    Node node = list->head->next;
    while (idx != position) {
        idx++;
        node = node->next;
    }
    return node->element;
}

/**
 * @brief Returns the position in the list of the first occurrence of the specified element.
 *
 * Returns -1 if the specified element does not occur in the list.
 *
 * @param list The linked list.
 * @param equal The function to compare two elements.
 * @param element The element to search for.
 * @return int The position in the list of the first occurrence of the specified element, or -1 if the specified element does not occur in the list.
 */
int list_find(List list, bool (*equal)(void*, void*), void* element) {
    Node node = list->head;
    int x = 0;
    while (node != NULL) {
        if (equal(node->element, element)) {
            return x;
        }
        node = node->next;
        x++;
    }
    return -1;
}

/**
 * @brief Inserts the specified element at the first position in the list.
 *
 * @param list The linked list.
 * @param element The element to insert.
 */
void list_insert_first(List list, void* element) {
    Node node = malloc(sizeof(struct Node_));
    node->element = element;
    node->next = list->head;
    list->head = node;
    if (list_is_empty(list)) {
        list->tail = list->head;
        // tail = node;
    }
    list->size++;
}

/**
 * @brief Inserts the specified element at the last position in the list.
 *
 * @param list The linked list.
 * @param element The element to insert.
 */
void list_insert_last(List list, void* element) {
    Node node = malloc(sizeof(struct Node_));
    node->element = element;
    node->next = NULL;
    if (list_is_empty(list)) {
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
    list->size++;
}

Node create_node(void* element, Node next) {
    Node node = malloc(sizeof(struct Node_));
    node->element = element;
    node->next = next;
    return node;
}

/**
 * @brief Inserts the specified element at the specified position in the list.
 *
 * Range of valid positions: 0, ..., size().
 * If the specified position is 0, insert corresponds to insertFirst.
 * If the specified position is size(), insert corresponds to insertLast.
 *
 * @param list The linked list.
 * @param element The element to insert.
 * @param position The position at which to insert the specified element.
 */
void list_insert(List list, void* element, int position) {
    if( position < 0 || position > list_size(list)){
        
    }
    if (position == 0) {
        list_insert_first(list, element);
    }
    else if(position == list_size(list)) {
        list_insert_last(list, element);
    }
    else {
        Node node = list->head;
        int idx = 0;
        while (idx != position - 1) {
            node = node->next;
            idx++;
        }
        Node new_node = create_node(element, node->next);
        node->next = new_node;
        list->size++;
    }
}



/**
 * @brief Removes and returns the element at the first position in the list.
 *
 * @param list The linked list.
 * @return void* The element at the first position in the list.
 */
void* list_remove_first(List list) {
    if(list_is_empty(list)) {
        return NULL;
    }
    Node node = list->head;
    list->head = node->next;
    list->size--;
    void* element = node->element;
    //free(node);
    if(list_size(list) == 0) {
        list->tail = list->head;
    }
    free_node(node);
    return element;
}

/**
 * @brief Removes and returns the element at the last position in the list.
 *
 * @param list The linked list.
 * @return void* The element at the last position in the list.
 */
void* list_remove_last(List list) {
    if(list_is_empty(list)) {
        return NULL;
    }
    Node node = list->head;
    Node prev = NULL;
    while (node->next != NULL) {
        prev = node;
        node = node->next;
    }
    list->tail = prev;
    list->size--;
    if(list_is_empty(list)) {
        list->head = list->tail;
    }
    list->tail->next = NULL;
    void* element = node->element;
    //free(node);
    free_node(node);
    return element;
}

/**
 * @brief Removes and returns the element at the specified position in the list.
 *
 * Range of valid positions: 0, ..., size()-1.
 *
 * @param list The linked list.
 * @param position The position of the element to remove.
 * @return void* The element at the specified position in the list.
 */
void* list_remove(List list, int position){
    if(list_is_empty(list)){
        return NULL;
    }
    if(position < 0 || position > list->size-1){
        return NULL;
    }
    if(position == 0){
        return list_remove_first(list);
    }
    if(position == list->size-1){
        return list_remove_last(list);
    }
    Node node = list->head;
    for(int i = 0; i < position-1; i++){
        node = node->next;
    }
    Node deleted_node = node->next; 
    node->next = node->next->next;
    list->size--;
    return free_node(deleted_node);

}

/**
 * @brief Removes all elements from the list.
 *
 * @param list The linked list.
 * @param free_element The function to free the elements of the list.
 */
void list_make_empty(List list, void (*free_element)(void*)) {
    while(!list_is_empty(list)){
        void* element = list_remove_first(list);
        if(free_element != NULL)
        free_element(element);
    }
}

// TODO só é necessário para o projeto até aqui

/**
 * @brief Returns an array with the elements of the list.
 *
 * @param list The linked list.
 * @param out_array The array to fill with the elements of the list.
 */

void list_to_array(List list, void** out_array) {
    Node node = list->head;
    int i=0;
    while(node != NULL){
        out_array[i] = node->element;
        node = node->next;
        i++;
    }
}

//converter list to array
/*
void list_to_array(List list, void** out_array) {
    User* user_array = malloc(sizeof(User) * size);
    list_iterator_start(user_list);
    int i= 0;
    while(list_iterator_has_next(user_list)) {
        User user = list_iterator_get_next(user_list);
        user_array[i++] = user;
    }
}
*/



/**
 * @brief Removes all duplicate occurrences of an element.
 *
 * Keeps only the first occurrence, and returns the number of occurrences.
 *
 * @param list The linked list.
 * @param equal_element The function to compare two elements.
 * @param free_element The function to free the elements of the list.
 * @param element The element to search for.
 * @return int The number of occurrences on an element.
 */
int list_remove_duplicates(List list, bool (*equal_element)(void*, void*), void (*free_element)(void*), void* element) {
    Node node = list->head;
    Node prev = NULL;
    int counter = 0;
    while (node != NULL) {
        if (equal_element(node->element, element)) {
            counter++;
            if (counter > 1) {
                // Atualizar o prev
                prev->next = node->next;
                if (node->next == NULL) {  // removing the tail
                    list->tail = prev;
                }
                // Remover!
                if (free_element != NULL) {
                    free_element(node->element);
                }
                free(node);
                node = prev->next;
            }
        }
        prev = node;
        node = node->next;
    }
    return counter;
}

// extra 

void list_iterator_start(List list) {
    list->iterator = list->head;
}
bool list_iterator_has_next(List list){
    return list->iterator != NULL;
}
void* list_iterator_get_next(List list) {
    void* element = list->iterator->element;
    list->iterator = list->iterator->next;
    return element;
}

void* free_node(Node node){
    void* element = node->element;
    free(node);
    return element;
}

