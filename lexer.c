#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "lexer.h"
#include "calc.lex.h"

char const* beg_lex = NULL;
char const* index_line = NULL;

static char* line = NULL;
static int peek = -1;

static int get_next_line(int, char**);

int
get_next_token(int filde) {
	if (!line) {
		if (get_next_line(filde, &line) == NO_READ)
			{ return (T_EOF); }
		index_line = line;
	}
	
	int last_match = T_ERROR;

	if (peek != -1) {
		last_match = peek;
		peek = -1;
		return (last_match);
	}
	if (!*index_line) {
		free(line);
		line = NULL;
		return (get_next_token(filde));
	}

	beg_lex = index_line;
	int state = START_STATE;

	while (*index_line && (state != DEAD_STATE)) {
		state = calc_state_table[state][*index_line];
		for (int i = 0; i < SIZE_FINAL_TAB; ++i) {
			if (*calc_final_table[i] == state)
				{ last_match = calc_final_table[i][1]; }
		}
		if (state != DEAD_STATE)
			{ ++index_line; }
	}
	return (last_match);
}

int
advance(int filde) {
	int token;
	while ((token = get_next_token(filde)) == T_SPACE);
	return (token);
}

int
peek_token(int filde) {
	if (peek == -1)
		{ peek = advance(filde); }
	return (peek);
}

static inline size_t
round_up(size_t base) {
	size_t power2 = 2;
	while (power2 <= base)
		{ power2 <<= 1; }
	return (power2);
}

static size_t
extend_line(char** line, size_t index, size_t crt_size) {
	if (index + 1 >= crt_size) {
		size_t new_sz = round_up(crt_size);
		if (!(*line = (char*)realloc(*line, new_sz)))
			{ return (0); }
		crt_size = new_sz;
	}
	return (crt_size);
}

int
get_next_line(int filde, char** line) {
	size_t crt_size = BUFF_SIZE;
	if (!line || !(*line = (char*)realloc(NULL, crt_size)))
		{ return (ERROR); }
	size_t index = 0;
	int end_file = NO_READ;
	while (1) {
		if (!(crt_size = extend_line(line, index, crt_size)))
			{ return (ERROR); }
		int rd;
		if ((rd = read(filde, (*line) + index, 1)) == -1)
			{ return (ERROR); }
		else if (!rd)
			{ return (end_file);  }
		else if ((*line)[index++] == '\n')
			{ return (READ); }
		end_file = READ;
	}
}

void
clean_up_lexer(void) {
	free(line);
}

