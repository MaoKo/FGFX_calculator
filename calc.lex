$TOKEN
{
	PLUS	= / "+"    / ;
	MINUS	= /  -     / ;
	MUL		= / "*"    / ;
	DIV		= / \/     / ;
	MOD		= /  %     / ;
	LPAREN	= / "("    / ;
	RPAREN	= / ")"    / ;
	NL		= / \n     / ;
	NUM		= / [0-9]+ / ;
};

$SKIP
{
	SPACE	= / [ \t]+ / ;
};
