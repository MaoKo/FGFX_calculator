#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "calc.lex.h"
#include "calc.grm.h"

#define BUFF_SIZE	(8)

#define READ		(1)
#define NO_READ		(0)
#define ERROR		(-1)

static char* line = NULL;

static char const* index_line = NULL;
static char const* beg_lex = NULL;

static int peek = -1;

static int get_next_line(int, char**);

int
get_next_token(void) {
	if (!line) {
		if (get_next_line(STDIN_FILENO, &line) == NO_READ)
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
		return (get_next_token());
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
advance(void) {
	int token;
	while ((token = get_next_token()) == T_SPACE);
	return (token);
}

int
peek_token(void) {
	if (peek == -1)
		{ peek = advance(); }
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

static int
lr_parser(void) {
	int value_stack[100];
	int value_top = 0;

	int parse_stack[100];
	int parse_top = 0;

	parse_stack[parse_top] = LR_START_STATE;

	//Parser Engine
	while (1) {
		int crt_token = peek_token();
		int top_state = parse_stack[parse_top];

		int kind = calc_lr_action_table[top_state][crt_token];

		if (kind == ACCEPT)
			{ return (EXIT_SUCCESS); }
		else if (kind & _SHIFT) {
			parse_stack[++parse_top] = kind ^ _SHIFT;

			if (crt_token == T_NUM)
				{ value_stack[value_top] = atoi(beg_lex); }

			++value_top;
			advance();
		}
		else if (kind & _REDUCE) {
			parse_top -= calc_rhs_prod_table[kind ^ _REDUCE];
	
			int uncovered_state = parse_stack[parse_top];
			int lhs_symbol = calc_lhs_prod_table[kind ^ _REDUCE];

			parse_stack[++parse_top] = 
					calc_lr_goto_table[uncovered_state][lhs_symbol] ^ _GOTO;

			//Place to put semantic action
			int tmp_val = value_stack[value_top - 1];
			switch (kind ^ _REDUCE) {
				case P_LHS_NLineExp_RHS_NE_TNL:
				case P_LHS_NLineExp_RHS_NLineExp_NE_TNL:
					printf("== %d\n", value_stack[value_top - 2]);
					break;

				case P_LHS_NE_RHS_NE_TPLUS_NE:
					tmp_val += value_stack[value_top - 3]; break;

				case P_LHS_NE_RHS_NE_TMINUS_NE:
					tmp_val -= value_stack[value_top - 3]; break;

				case P_LHS_NE_RHS_NE_TMUL_NE:
					tmp_val *= value_stack[value_top - 3]; break;

				case P_LHS_NE_RHS_NE_TDIV_NE:
					if (!tmp_val) {
						fprintf(stderr, "Try to divide by zero.\n");
						return (EXIT_FAILURE);
					}
					tmp_val = value_stack[value_top - 3] / tmp_val;
					break;

				case P_LHS_NE_RHS_NE_TMOD_NE:
					if (!tmp_val) {
						fprintf(stderr, "Try to do  a modulo by zero.\n");
						return (EXIT_FAILURE);
					}
					tmp_val = value_stack[value_top - 3] % tmp_val;
					break;

/*
				case P_LHS_NE_RHS_NE_TEXPO_NE:
					tmp_val = pow(value_stack[value_top - 3], tmp_val); break;

				case P_LHS_NE_RHS_TPLUS_NE:
					tmp_val = + tmp_val; break;

				case P_LHS_NE_RHS_TMINUS_NE:
					tmp_val = - tmp_val; break;
*/

				case P_LHS_NE_RHS_TLPAREN_NE_TRPAREN:
						tmp_val = value_stack[value_top - 2]; break;

/*
				case P_LHS_NE_RHS_TID:
					crt_entry = entry_at(tmp_val);
					if (!crt_entry->is_defined) {
						fprintf(stderr, "Symbol %s is not defined.\n",
														crt_entry->ident);
						return (EXIT_FAILURE);
					}
					tmp_val = crt_entry->value;
					break;
*/
			}
			value_top -= calc_rhs_prod_table[kind ^ _REDUCE];
			value_stack[value_top++] = tmp_val;
		}
		else {
			fprintf(stderr, "Error with the token %d.\n", crt_token);
			return (EXIT_FAILURE);
		}
	}
}

int
main(void) {
	lr_parser(); free(line);
	return (EXIT_SUCCESS);
}
