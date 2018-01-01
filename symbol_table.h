#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

typedef struct symbol_entry_t {
	struct symbol_entry_t* next;
	int index;
	char* ident;
	int value;
	bool is_defined;
} symbol_entry_t;

void del_symbol_table(symbol_entry_t*);
int new_entry(symbol_entry_t**);
symbol_entry_t* entry_at(symbol_entry_t*, int);

#endif /* SYMBOL_TABLE_H */
