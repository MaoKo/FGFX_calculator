#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"
#include "symbol_table.h"
#include "calc.grm.h"

static symbol_entry_t* symbol_table = NULL;

int
lr_parser(int filde) {
	int value_stack[100];
	int value_top = 0;

	int parse_stack[100];
	int parse_top = 0;

	parse_stack[parse_top] = LR_START_STATE;
	//Parser Engine
	while (1) {
		int crt_token = peek_token(filde);
		int top_state = parse_stack[parse_top];

		int kind = calc_lr_action_table[top_state][crt_token];

		if (kind == ACCEPT)
			{ return (EXIT_SUCCESS); }
		else if (kind & _SHIFT) {
			parse_stack[++parse_top] = kind ^ _SHIFT;

			if (crt_token == T_NUM)
				{ value_stack[value_top] = atoi(beg_lex); }
			else if (crt_token == T_ID) {
				if ((value_stack[value_top] = new_entry(&symbol_table))
						== ERROR) {
					fprintf(stderr, "Unable to allocate enough memory :(.\n");
					return (EXIT_FAILURE);
				}
			}		

			++value_top;
			advance(filde);
		}
		else if (kind & _REDUCE) {
			parse_top -= calc_rhs_prod_table[kind ^ _REDUCE];
	
			int uncovered_state = parse_stack[parse_top];
			int lhs_symbol = calc_lhs_prod_table[kind ^ _REDUCE];

			parse_stack[++parse_top] = 
					calc_lr_goto_table[uncovered_state][lhs_symbol] ^ _GOTO;

			//Place to put semantic action
			int tmp_val = value_stack[value_top - 1];
			symbol_entry_t* crt_entry = NULL;

			switch (kind ^ _REDUCE) {
				case P_LHS_NE_RHS_TID_TEQUAL_NE:
					tmp_val = value_stack[value_top - 3];

					crt_entry = entry_at(symbol_table, tmp_val);
					crt_entry->value = value_stack[value_top - 1];
					crt_entry->is_defined = true;

					printf("-> %d\n", crt_entry->value);
					tmp_val = crt_entry->value;
					break;

				case P_LHS_NLineExp_RHS_NLineExp_NE_TNL:
				case P_LHS_NLineExp_RHS_NE_TNL:
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

				case P_LHS_NE_RHS_TID:
					crt_entry = entry_at(symbol_table, tmp_val);
					if (!crt_entry->is_defined) {
						fprintf(stderr, "Symbol %s is not defined.\n",
														crt_entry->ident);
						return (EXIT_FAILURE);
					}
					tmp_val = crt_entry->value;
					break;
			}
			value_top -= calc_rhs_prod_table[kind ^ _REDUCE];
			value_stack[value_top++] = tmp_val;
		}
		else {
			fprintf(stderr, "error with the token %d.\n", crt_token);
			return (EXIT_FAILURE);
		}
	}
}

void
clean_up_parser(void) {
	del_symbol_table(symbol_table);
	symbol_table = NULL;
}

