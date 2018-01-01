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
	T_EXPO,
	T_LPAREN,
	T_RPAREN,
	T_NL,
	T_EQUAL,
	T_NUM,
	T_ID,
	T_SPACE,
	T_ERROR,
	T_EOF,
};

#define TOTAL_TOKEN	15

#ifndef _ONLY_TOKEN_

#define START_STATE	1
#define DEAD_STATE	0

static dfa_state_t
calc_state_table[15][256] = {
/*   0 */	{},
/*   1 */	{[97 ... 122]=13, [94]=14, [65 ... 90]=13, [61]=12, [48 ... 57]=11, [47]=10, [45]=9, [43]=8, [42]=7, [41]=6, [40]=5, [37]=4, [32]=2, [10]=3, [9]=2},
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
/*  12 */	{},
/*  13 */	{[97 ... 122]=13, [65 ... 90]=13},
/*  14 */	{},
};

#define SIZE_FINAL_TAB	13

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
	{ 12, 	T_EQUAL },
	{ 13, 	T_ID },
	{ 14, 	T_EXPO },
};

static int
calc_skip_table[2] = {
	T_SPACE,
	-1,
};

#endif /* _ONLY_TOKEN_ */
#endif /* _CALC_LEX_H_ */
