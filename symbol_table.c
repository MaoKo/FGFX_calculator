#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "symbol_table.h"
#include "lexer.h"

void
del_symbol_table(symbol_entry_t* symbol_table) {
	while (symbol_table) {
		free(symbol_table->ident);
		symbol_entry_t* next = symbol_table->next;
		free(symbol_table);
		symbol_table = next;
	}
}

int
new_entry(symbol_entry_t** symbol_table) {
	static int index = 0;
	if (!symbol_table)
		{ return (ERROR); }

	size_t size_lexeme = index_line - beg_lex;
	symbol_entry_t* entry = (*symbol_table);

	while (entry) {
		if (!strncmp(entry->ident, beg_lex, size_lexeme))
			{ return (entry->index); }
		entry = entry->next;
	}

	entry = (symbol_entry_t*)malloc(sizeof(*entry));
	if (!entry)
		{ return (ERROR); }

	entry->index = index++;
	if (!(entry->ident = (char*)malloc(sizeof(*entry->ident) * size_lexeme)))
		{ return (ERROR); }

	strncpy(entry->ident, beg_lex, size_lexeme);

	entry->next = (*symbol_table);
	(*symbol_table) = entry;

	return (entry->index);
}

symbol_entry_t*
entry_at(symbol_entry_t* symbol_table, int index) {
	symbol_entry_t* crt_entry = symbol_table;
	while (crt_entry) {
		if (crt_entry->index == index)
			{ break; }
		crt_entry = crt_entry->next;
	}
	return (crt_entry);
}
