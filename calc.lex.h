#ifndef _CALC_LEX_H_
#define _CALC_LEX_H_

#include <stdint.h>
typedef	uint8_t	dfa_state_t;
typedef	uint8_t	final_state_t;

enum {
	T_PLUS,
	T_MINUS,
	T_MUL,
	T_DIV,
	T_MOD,
	T_LPAREN,
	T_RPAREN,
	T_NL,
	T_NUM,
	T_SPACE,
	T_ERROR,
	T_EOF,
};

#define TOTAL_TOKEN	12

#ifndef _ONLY_TOKEN_

#define START_STATE	1
#define DEAD_STATE	0

static dfa_state_t
calc_state_table[12][256] = {
/*   0 */	{},
/*   1 */	{[48 ... 57]=11, [47]=10, [45]=9, [43]=8, [42]=7, [41]=6, [40]=5, [37]=4, [32]=2, [10]=3, [9]=2},
/*   2 */	{[32]=2, [9]=2},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{},
/*  10 */	{},
/*  11 */	{[48 ... 57]=11},
};

#define SIZE_FINAL_TAB	10

static final_state_t
calc_final_table[SIZE_FINAL_TAB][2] = {
	{ 2, 	T_SPACE },
	{ 3, 	T_NL },
	{ 4, 	T_MOD },
	{ 5, 	T_LPAREN },
	{ 6, 	T_RPAREN },
	{ 7, 	T_MUL },
	{ 8, 	T_PLUS },
	{ 9, 	T_MINUS },
	{ 10, 	T_DIV },
	{ 11, 	T_NUM },
};

static int
calc_skip_table[2] = {
	T_SPACE,
	-1,
};

#endif /* _ONLY_TOKEN_ */
#endif /* _CALC_LEX_H_ */
