digit	[0-9]
letter	[A-Za-z]

%{
	#ifdef _MSC_VER
		// Disable spurious VC++ warnings
		#pragma warning(disable:4047 4244 4267 4996)
	#endif

	#include <stdlib.h>
	#include <string.h>
	#include "yypropsimplifier.tab.h"

	extern int yypropsimplifiernerrs;
	void yypropsimplifiererror(char *);
%}

%%

-?{digit}+(\.{digit}+)?		{ yypropsimplifierlval = strdup(yytext); return NUMBER; }
\"[^\"]*\"					{ yypropsimplifierlval = strdup(yytext); return STRING; }
\([ ]						{ return VECLBRACKET; }
[ ]\)						{ return VECRBRACKET; }

[()\[\]<>\.,;]				return *yytext;

[ \t]						; /* skip whitespace */

.							{
								char *str;
								++yypropsimplifiernerrs;
								str = malloc(strlen("invalid character: ") + 2);
								sprintf(str, "invalid character: %c", *yytext);
								yypropsimplifiererror(str);
								free(str);
							}

%%

int yywrap(void)
{
	return 1;
}
