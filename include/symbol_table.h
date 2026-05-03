/**
 * @file symbol_table.h
 * @brief A generic Symbol Table Abstract Data Type (ADT).
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/**
 * @brief Opaque handle for the Symbol Table.
 */
typedef struct SymbolTable *SymbolTable_T;

/**
 * @brief Creates a new, empty Symbol Table.
 * @return A handle to the new Symbol Table.
 */
SymbolTable_T SymbolTable_new(void);

/**
 * @brief Frees all memory associated with the Symbol Table.
 * @param st The Symbol Table to free.
 */
void SymbolTable_free(SymbolTable_T st);

/**
 * @brief Returns the number of bindings in the Symbol Table.
 * @param st The Symbol Table.
 * @return Number of bindings.
 */
unsigned int SymbolTable_get_length(SymbolTable_T st);

/**
 * @brief Adds a new binding to the Symbol Table.
 * @param st The Symbol Table.
 * @param key The unique key for the binding (string).
 * @param value The value associated with the key.
 * @return 1 if successfully added, 0 if key already exists.
 */
int SymbolTable_put(SymbolTable_T st, const char *key, const void *value);

/**
 * @brief Removes a binding from the Symbol Table.
 * @param st The Symbol Table.
 * @param key The key to remove.
 * @return 1 if successfully removed, 0 if key not found.
 */
int SymbolTable_remove(SymbolTable_T st, const char *key);

/**
 * @brief Checks if the Symbol Table contains a binding for a key.
 * @param st The Symbol Table.
 * @param key The key to check.
 * @return 1 if found, 0 otherwise.
 */
int SymbolTable_contains(SymbolTable_T st, const char *key);

/**
 * @brief Retrieves the value associated with a key.
 * @param st The Symbol Table.
 * @param key The key to look up.
 * @return The value, or NULL if not found.
 */
void *SymbolTable_get(SymbolTable_T st, const char *key);

/**
 * @brief Applies a function to all bindings in the Symbol Table.
 * @param st The Symbol Table.
 * @param apply Function to call for each binding.
 * @param extra Extra pointer passed to the apply function.
 */
void SymbolTable_map(SymbolTable_T st, 
                     void (*apply)(const char *key, void *value, void *extra),
                     const void *extra);

#endif /* SYMBOL_TABLE_H */
