#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#include "list.h"

typedef struct {
    void* key;
    void* value;
    bool (*key_equal)(void*, void*);
    void (*key_destroy)(void*);
    void (*value_destroy)(void*);
} t_Item, *Item;

struct t_HashTable {
    int size;
    int num_elements;
    List* table;
    bool (*key_equal)(void*, void*);
    void (*key_destroy)(void*);
    void (*value_destroy)(void*);
    int (*hash)(void*, int);
};

int _default_hash(void* key, int size) {
    char* k = (char*)key;
    int hash = 0;
    int a = 127;
    for (size_t i = 0; i < strlen(k); i++) {
        hash = (hash * a + k[i]) % size;
    }
    return hash;
}

bool _default_key_equal(void* key1, void* key2) {
    char* k1 = (char*)key1;
    char* k2 = (char*)key2;
    return strcmp(k1, k2) == 0;
}

bool _equal_item(void* item1, void* item2) {
    Item i1 = (Item)item1;
    Item i2 = (Item)item2;
    return i1->key_equal(i1->key, i2->key);
}

Item _item_create(HashTable htable, void* key, void* value) {
    Item item = malloc(sizeof(t_Item));
    item->key = key;
    item->value = value;
    item->key_equal = htable->key_equal;
    item->key_destroy = htable->key_destroy;
    item->value_destroy = htable->value_destroy;
    return item;
}

void _destroy_item(void* item, void (*key_destroy)(void*), void (*value_destroy)(void*)) {
    Item i = (Item)item;
    if (key_destroy != NULL) {
        key_destroy(i->key);
    }
    if (value_destroy != NULL) {
        value_destroy(i->value);
    }
    free(i);
}

void _collision_list_destroy_item(void* item) {
    Item i = (Item)item;
    if (i->key_destroy != NULL) {
        i->key_destroy(i->key);
    }
    if (i->value_destroy != NULL) {
        i->value_destroy(i->value);
    }
    free(i);
}

List _get_key_list(HashTable htable, void* key, bool* has_key, int* position) {
    int index = htable->hash(key, htable->size) % htable->size;
    List list = htable->table[index];
    *has_key = false;
    Item item = _item_create(htable, key, NULL);
    *position = list_find(list, _equal_item, item);
    _destroy_item(item, NULL, NULL);
    if (*position != -1) {
        *has_key = true;
        return list;
    }
    return list;
}

HashTable hash_table_create(int size, int (*hash)(void*, int), bool (*key_equal)(void*, void*), void (*key_destroy)(void*), void (*value_destroy)(void*)) {
    HashTable htable = malloc(sizeof(struct t_HashTable));
    htable->num_elements = 0;
    if (size <= 0) {
        htable->size = DEFAULT_SIZE;
    } else {
        htable->size = size;
    }
    if (hash == NULL) {
        htable->hash = _default_hash;
    } else {
        htable->hash = hash;
    }
    if (key_equal == NULL) {
        htable->key_equal = _default_key_equal;
    } else {
        htable->key_equal = key_equal;
    }
    htable->key_destroy = key_destroy;
    htable->value_destroy = value_destroy;

    htable->table = malloc(sizeof(List) * htable->size);
    for (int i = 0; i < htable->size; i++) {
        htable->table[i] = list_create();
    }
    return htable;
}

void hash_table_destroy(HashTable htable) {
    for (int i = 0; i < htable->size; i++) {
        List list = htable->table[i];
        list_destroy(list, _collision_list_destroy_item);
    }
    free(htable->table);
    free(htable);
}

void hash_table_insert(HashTable htable, void* key, void* value) {
    bool has_key;
    int position;
    List list = _get_key_list(htable, key, &has_key, &position);
    if (!has_key) {
        Item item = _item_create(htable, key, value);
        list_insert_last(list, item);
        htable->num_elements++;
    }
}

void* hash_table_remove(HashTable htable, void* key) {
    bool has_key;
    int position;
    List list = _get_key_list(htable, key, &has_key, &position);
    if (!has_key) {
        return NULL;
    }
    Item item = list_remove(list, position);
    htable->num_elements--;
    void* value = item->value;
    free(item);
    return value;
}

void*  hash_table_get(HashTable htable, void* key) {
    bool has_key;
    int position;
    List list = _get_key_list(htable, key, &has_key, &position);
    if (!has_key) {
        return NULL;
    }
    Item item = list_get(list, position);
    return item->value;
}

void* hash_table_update(HashTable htable, void* key, void* value) {
    bool has_key;
    int position;
    List list = _get_key_list(htable, key, &has_key, &position);
    if (!has_key) {
        return NULL;
    }
    Item item = list_get(list, position);
    void* old_value = item->value;
    item->value = value;
    return old_value;
}

int hash_table_size(HashTable htable) {
    return htable->num_elements;
}

bool hash_table_is_empty(HashTable htable) {
    return hash_table_size(htable) == 0;
}

List hash_table_keys(HashTable htable) {
    List keys = list_create();
    for (int i = 0; i < htable->size; i++) {
        List list = htable->table[i];
        list_iterator_start(list);
        while (list_iterator_has_next(list)) {
            Item item = list_iterator_get_next(list);
            list_insert_last(keys, item->key);
        }
    }
    return keys;
}

List hash_table_values(HashTable htable) {
    List values = list_create();
    for (int i = 0; i < htable->size; i++) {
        List list = htable->table[i];
        list_iterator_start(list);
        while (list_iterator_has_next(list)) {
            Item item = list_iterator_get_next(list);
            list_insert_last(values, item->value);
        }
    }
    return values;
}

void hash_table_rehash(HashTable htable, int new_size) {
    if (new_size <= 0) {
        return;
    }
    HashTable new_htable = hash_table_create(new_size, htable->hash, htable->key_equal, htable->key_destroy, htable->value_destroy);
    for (int i = 0; i < htable->size; i++) {
        List list = htable->table[i];
        list_iterator_start(list);
        while (list_iterator_has_next(list)) {
            Item item = list_iterator_get_next(list);
            hash_table_insert(new_htable, item->key, item->value);
        }
    }
    hash_table_destroy(htable);
    htable->size = new_htable->size;
    htable->num_elements = new_htable->num_elements;
    htable->table = new_htable->table;
    free(new_htable);
}


