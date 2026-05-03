#include "symbol_table.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_basic_ops() {
    SymbolTable_T st = SymbolTable_new();
    int val1 = 100, val2 = 200;

    assert(SymbolTable_put(st, "key1", &val1) == 1);
    assert(SymbolTable_put(st, "key2", &val2) == 1);
    assert(SymbolTable_put(st, "key1", &val1) == 0); /* Duplicate */

    assert(SymbolTable_get_length(st) == 2);
    assert(*(int*)SymbolTable_get(st, "key1") == 100);
    assert(SymbolTable_contains(st, "key2") == 1);

    assert(SymbolTable_remove(st, "key1") == 1);
    assert(SymbolTable_get_length(st) == 1);
    assert(SymbolTable_get(st, "key1") == NULL);

    SymbolTable_free(st);
    printf("test_basic_ops passed!\n");
}

void test_rehashing() {
    SymbolTable_T st = SymbolTable_new();
    char keys[600][10];
    int values[600];

    /* Force a rehash by inserting more than 509 elements */
    for (int i = 0; i < 600; i++) {
        sprintf(keys[i], "k%d", i);
        values[i] = i;
        SymbolTable_put(st, keys[i], &values[i]);
    }

    assert(SymbolTable_get_length(st) == 600);
    assert(*(int*)SymbolTable_get(st, "k599") == 599);

    SymbolTable_free(st);
    printf("test_rehashing passed!\n");
}

int main() {
    test_basic_ops();
    test_rehashing();
    printf("\nAll Symbol Table tests passed successfully!\n");
    return 0;
}
