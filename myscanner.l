%{
	#include "y.tab.h"
	#include <string.h>
	#include <stdio.h>
%}

%%
[1-9][0-9]*		{yylval.s = strdup(yytext); return INT;}
[0-9]*\.[0-9]+		{yylval.s = strdup(yytext);return FLOAT;}
[a-zA-Z][_a-zA-Z0-9]*	{yylval.s = strdup(yytext);return ID;}
[ \t\n]			;
.				{return yytext[0];}
%%

int yywrap(void)
{
	return 1;
}
