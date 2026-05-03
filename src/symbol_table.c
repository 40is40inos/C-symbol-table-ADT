/**
 * @file symbol_table.c
 * @brief Hash-table based implementation of the Symbol Table ADT.
 */

#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define HASH_MULTIPLIER 65599

/**
 * @brief Predefined bucket sizes for the hash table to maintain efficiency.
 */
static const unsigned int BUCKET_SIZES[] = {
    509, 1021, 2053, 4093, 8191, 16381, 32771, 65521
};

#define MAX_BUCKET_SIZE_INDEX 7

typedef struct Node {
    char *key;
    void *value;
    struct Node *next;
} *Node_T;

struct SymbolTable {
    Node_T *buckets;
    unsigned int length;
    unsigned int bucket_index;
};

/**
 * @brief Hash function for strings.
 */
static unsigned int hash(const char *key) {
    size_t i;
    unsigned int h = 0U;
    assert(key);
    for (i = 0U; key[i] != '\0'; i++) {
        h = h * HASH_MULTIPLIER + (unsigned char)key[i];
    }
    return h;
}

/**
 * @brief Internal helper to duplicate a string.
 */
static char *st_strdup(const char *s) {
    char *d = malloc(strlen(s) + 1);
    if (d) strcpy(d, s);
    return d;
}

/**
 * @brief Rehashes the symbol table to a larger size.
 */
static void rehash(SymbolTable_T st) {
    unsigned int old_size, new_size, i, new_h;
    Node_T *old_buckets, *new_buckets, curr, next;

    if (st->bucket_index >= MAX_BUCKET_SIZE_INDEX) return;

    old_size = BUCKET_SIZES[st->bucket_index];
    st->bucket_index++;
    new_size = BUCKET_SIZES[st->bucket_index];

    new_buckets = calloc(new_size, sizeof(Node_T));
    if (!new_buckets) {
        st->bucket_index--; /* Revert if allocation fails */
        return;
    }

    old_buckets = st->buckets;
    for (i = 0; i < old_size; i++) {
        curr = old_buckets[i];
        while (curr) {
            next = curr->next;
            new_h = hash(curr->key) % new_size;
            curr->next = new_buckets[new_h];
            new_buckets[new_h] = curr;
            curr = next;
        }
    }

    st->buckets = new_buckets;
    free(old_buckets);
}

SymbolTable_T SymbolTable_new(void) {
    SymbolTable_T st = malloc(sizeof(struct SymbolTable));
    if (!st) return NULL;

    st->bucket_index = 0;
    st->length = 0;
    st->buckets = calloc(BUCKET_SIZES[0], sizeof(Node_T));
    if (!st->buckets) {
        free(st);
        return NULL;
    }

    return st;
}

void SymbolTable_free(SymbolTable_T st) {
    unsigned int i, size;
    Node_T curr, next;

    if (!st) return;

    size = BUCKET_SIZES[st->bucket_index];
    for (i = 0; i < size; i++) {
        curr = st->buckets[i];
        while (curr) {
            next = curr->next;
            free(curr->key);
            free(curr);
            curr = next;
        }
    }
    free(st->buckets);
    free(st);
}

unsigned int SymbolTable_get_length(SymbolTable_T st) {
    assert(st);
    return st->length;
}

int SymbolTable_put(SymbolTable_T st, const char *key, const void *value) {
    unsigned int h, size;
    Node_T curr, newNode;

    assert(st && key);

    size = BUCKET_SIZES[st->bucket_index];
    h = hash(key) % size;

    curr = st->buckets[h];
    while (curr) {
        if (strcmp(curr->key, key) == 0) return 0;
        curr = curr->next;
    }

    /* Check if expansion is needed */
    if (st->length >= size && st->bucket_index < MAX_BUCKET_SIZE_INDEX) {
        rehash(st);
        size = BUCKET_SIZES[st->bucket_index];
        h = hash(key) % size;
    }

    newNode = malloc(sizeof(struct Node));
    if (!newNode) return 0;

    newNode->key = st_strdup(key);
    if (!newNode->key) {
        free(newNode);
        return 0;
    }
    newNode->value = (void *)value;
    newNode->next = st->buckets[h];
    st->buckets[h] = newNode;
    st->length++;

    return 1;
}

int SymbolTable_remove(SymbolTable_T st, const char *key) {
    unsigned int h, size;
    Node_T curr, prev = NULL;

    assert(st && key);

    size = BUCKET_SIZES[st->bucket_index];
    h = hash(key) % size;

    curr = st->buckets[h];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) prev->next = curr->next;
            else st->buckets[h] = curr->next;

            free(curr->key);
            free(curr);
            st->length--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }

    return 0;
}

int SymbolTable_contains(SymbolTable_T st, const char *key) {
    unsigned int h, size;
    Node_T curr;

    assert(st && key);

    size = BUCKET_SIZES[st->bucket_index];
    h = hash(key) % size;

    curr = st->buckets[h];
    while (curr) {
        if (strcmp(curr->key, key) == 0) return 1;
        curr = curr->next;
    }

    return 0;
}

void *SymbolTable_get(SymbolTable_T st, const char *key) {
    unsigned int h, size;
    Node_T curr;

    assert(st && key);

    size = BUCKET_SIZES[st->bucket_index];
    h = hash(key) % size;

    curr = st->buckets[h];
    while (curr) {
        if (strcmp(curr->key, key) == 0) return curr->value;
        curr = curr->next;
    }

    return NULL;
}

void SymbolTable_map(SymbolTable_T st, 
                     void (*apply)(const char *key, void *value, void *extra),
                     const void *extra) {
    unsigned int i, size;
    Node_T curr, next;

    assert(st && apply);

    size = BUCKET_SIZES[st->bucket_index];
    for (i = 0; i < size; i++) {
        curr = st->buckets[i];
        while (curr) {
            next = curr->next; /* Allow removal during map if needed, though not strictly required by API */
            apply(curr->key, curr->value, (void *)extra);
            curr = next;
        }
    }
}
