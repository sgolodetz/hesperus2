%error-verbose

%token NUMBER STRING
%token VECLBRACKET VECRBRACKET

%{
	#ifdef _MSC_VER
		// Disable spurious VC++ warnings
		#pragma warning(disable:4024 4047 4273 4312 4996)
	#endif

	#include <malloc.h>
	#include <stdio.h>
	#include <string.h>

	int yypropsimplifierlex(void);
	void yypropsimplifiererror(char *);

	/* Custom function, not part of Bison */
	void yypropsimplifieroutput(const char *s);
%}

%%

start:
	value										{ yypropsimplifieroutput($1); }
	;

value:
	NUMBER
	| STRING									{
													size_t len = strlen($1) - 2;

													/* Note: We leave a reasonable space to store the length. */
													char *str = malloc(len + 7);
													sprintf(str, "<%i> ", len);
													strncat(str, &((char*)($1))[1], len);
													$$ = str;
												}
	| value_array
	| value_map
	| value_pair
	| value_triple
	| value_vector3
	;

values:
	value ',' values							{
													char *str;

													if(yynerrs != 0) return 1;

													str = malloc(strlen($1) + strlen($3) + 2);
													sprintf(str, "%s %s", $1, $3);
													$$ = str;
												}
	| value										{ $$ = $1; }
	|											{ $$ = ""; }
	;

value_array:
	'[' values ']'								{
													char *str;

													if(yynerrs != 0) return 1;

													/* Note: We leave a reasonable space to store the length. */
													str = malloc(strlen($2) + 10);
													sprintf(str, "<%i> %s", strlen($2), $2);
													$$ = str;
												}
	;

value_map:
	'<' value_pairs '>'							{
													char *str;

													if(yynerrs != 0) return 1;

													/* Note: We leave a reasonable space to store the length. */
													str = malloc(strlen($2) + 10);
													sprintf(str, "<%i> %s", strlen($2), $2);
													$$ = str;
												}
	;

value_pair:
	'(' value ',' value ')'						{
													size_t len;
													char *str;

													if(yynerrs != 0) return 1;

													/* Note: We leave a reasonable space to store the length. */
													len = strlen($2) + strlen($4) + 1;
													str = malloc(len + 10);
													sprintf(str, "<%i> %s %s", len, $2, $4);
													$$ = str;
												}
	;

value_pairs:
	value_pair ';' value_pairs					{
													char *str;

													if(yynerrs != 0) return 1;

													str = malloc(strlen($1) + strlen($3) + 2);
													sprintf(str, "%s %s", $1, $3);
													$$ = str;
												}
	| value_pair								{ $$ = $1; }
	|											{ $$ = ""; }
	;

value_triple:
	'(' value ',' value ',' value ')'			{
													size_t len;
													char *str;

													if(yynerrs != 0) return 1;

													/* Note: We leave a reasonable space to store the length. */
													len = strlen($2) + strlen($4) + strlen($6) + 2;
													str = malloc(len + 10);
													sprintf(str, "<%i> %s %s %s", len, $2, $4, $6);
													$$ = str;
												}
	;

value_vector3:
	VECLBRACKET value value value VECRBRACKET	{
													size_t len;
													char *str;

													if(yynerrs != 0) return 1;

													/* Note: We leave a reasonable space to store the length. */
													len = strlen($2) + strlen($3) + strlen($4) + 2;
													str = malloc(len + 10);
													sprintf(str, "<%i> %s %s %s", len, $2, $3, $4);
													$$ = str;
												}
	;

%%
