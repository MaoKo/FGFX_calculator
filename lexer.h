#ifndef LEXER_H
#define LEXER_H

#define BUFF_SIZE	(8)

#define READ		(1)
#define NO_READ		(0)
#define ERROR		(-1)

extern char const* beg_lex;
extern char const* index_line;

int get_next_token(int);
int advance(int);
int peek_token(int);
void clean_up_lexer(void);

#endif /* LEXER_H */
