%{
	#include "y.tab.h"
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	extern FILE * yyin;
%}

%union {char* s;}
%token <s> FLOAT INT ID

%%
/*descriptions of expected inputs	corresponding actions (in C)*/
P		: P S 			{printf("P->PS\n");}
		| S 			{printf("P->S\n");}
		;
S		: LHS '=' RHS ';' 	{printf("LHS->RHS; \n");}
		;
LHS		: TRef			{printf("LHS->TRef\n");}
		;
RHS		: RHS '+' RHS		{printf("RHS->RHS + RHS\n");}
		| RHS '*' RHS		{printf("RHS->RHS * RHS\n");}
		| RHS '-' RHS		{printf("RHS->RHS - RHS\n");}
		| RHS '/' RHS		{printf("RHS->RHS / RHS\n");}
		| RHS '%' RHS		{printf("RHS->RHS % RHS\n");}
		| RHS '/' '/' RHS		{printf("RHS->RHS \/\/ RHS\n");}
		| '(' RHS ')'		{printf("RHS->(RHS)\n");}
		| TRef				{printf("RHS->TRef\n");}
		| SRef				{printf("RHS->SRef\n");}
		| Const				{printf("RHS->Const\n");}
		;
TRef	: ID '<' CList '>' '[' AList ']'	{printf("TRef->Id<CList>[AList]\t%s\n",$1);}
		;
SRef	: ID '<' CList '>'	{printf("SRef->Id<CList>\t%s\n",$1);}
		;
CList	: CList ','	INT	{printf("CList->CList,INT\t%s\n",$3);}
		| INT				{printf("CList->INT\t%s\n",$1);}
		;
AList	: AList ',' IdExpr	{printf("AList->AList,IdExpr\n");}
		| IdExpr			{printf("AList->IdExpr\n");}
		;
IdExpr	: ID				{printf("IdExpr->ID\t%s\n",$1);}
		| IdExpr '+' IdExpr	{printf("IdExpr->IdExpr+IdExpr\n");}
		| IdExpr '+' INT	{printf("IdExpr->IdExpr+INT\t%s\n",$3);}
		| IdExpr '*' INT	{printf("IdExpr->IdExpr*INT\t%s\n",$3);}
		| IdExpr '/' '/' INT	{printf("IdExpr->IdExpr\/\/INT\t%s\n",$4);}
		| IdExpr '%' INT	{printf("IdExpr->IdExpr%INT\t%s\n",$3);}
		| '(' IdExpr ')'	{printf("IdExpr->(IdExpr)\n");}
Const	: FLOAT				{printf("Const->FLOAT\t%s\n",$1);}
		| INT				{printf("Const->INT\t%s\n",$1);}
%%
int yy_main(){
	yyin = fopen("./temp/case_string", "r+");
	yyparse();
	return 0;
}
int yyerror(char *msg)
{
printf("%s\n", msg);
}
