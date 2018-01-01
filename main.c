#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

void
clean_up(void) {
	clean_up_lexer();
	clean_up_parser();
}

int
main(int argc, char const* argv[]) {
	int filde = STDIN_FILENO;
	if (argc == 2) {
		filde = open(argv[1], O_RDONLY);
		if (filde == -1)
			{ exit(EXIT_FAILURE); }
	}
	int exit_st = lr_parser(filde);
	clean_up();
	return (exit_st);
}
