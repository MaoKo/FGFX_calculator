$TOKEN
{
	PLUS	= / "+"       / ;
	MINUS	= /  -        / ;
	MUL		= / "*"       / ;
	DIV		= / \/        / ;
	MOD		= /  %        / ;
	LPAREN	= / "("       / ;
	RPAREN	= / ")"       / ;
	NL		= / \n        / ;
	EQUAL	= /  =        / ;
	NUM		= / [0-9]+    / ;
	ID		= / [A-Za-z]+ / ;
};

$SKIP
{
	SPACE	= / [ \t]+    / ;
};
