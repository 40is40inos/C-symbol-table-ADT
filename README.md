# C Symbol Table ADT

A robust, generic Symbol Table (Dictionary) Abstract Data Type implemented in C. This project provides a flexible way to store and manage key-value pairs (where keys are strings and values are arbitrary pointers) using an efficient hash table with dynamic resizing.

## Features

- **Generic Values**: Stores `void *` values, allowing any data type to be associated with a string key.
- **Efficient Performance**: 
  - Hash table implementation for O(1) average-case time complexity for most operations.
  - **Dynamic Resizing**: Automatically rehashes to a larger bucket size as the number of entries grows to maintain performance.
- **Robust API**:
  - `SymbolTable_new`: Create a new table.
  - `SymbolTable_put`: Add or update bindings.
  - `SymbolTable_get`: Retrieve values.
  - `SymbolTable_remove`: Delete bindings.
  - `SymbolTable_contains`: Check for key existence.
  - `SymbolTable_map`: Iterate over all bindings with a callback function.
- **Memory Safety**: Clean management of internal keys and structures.

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`, `clang`)
- `make` build tool

### Compilation

Build the library:

```bash
make
```

## Usage Example

```c
#include "symbol_table.h"
#include <stdio.h>

void print_binding(const char *key, void *value, void *extra) {
    printf("%s: %s\n", key, (char *)value);
}

int main() {
    SymbolTable_T st = SymbolTable_new();
    
    SymbolTable_put(st, "username", "john_doe");
    SymbolTable_put(st, "email", "john@example.com");
    
    char *user = SymbolTable_get(st, "username");
    printf("User: %s\n", user);
    
    SymbolTable_map(st, print_binding, NULL);
    
    SymbolTable_free(st);
    return 0;
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
