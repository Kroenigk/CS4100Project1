%{
/* ===== C DECLARATIONS ===== */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <string.h>

using namespace std;

#define TKVARIABLE 001
#define TKPOINTER 002
#define TKIF 003
#define TKFOR 004
#define TKWHILE 005
#define TKRETURN 006
#define TKELSE 007
#define TKDO 010
#define TKVOID 011
#define TKINTKW 012
#define TKCHARKW 013
#define TKSTRINGKW 014
#define TKBOOLKW 015
#define TKPUNC 016
#define TKNUM 017
#define TKCHAR 020
#define TKSTRING 021
#define TKBOOL 022
#define TKOPERATOR 023
#define TKUNMATCHED 255

typedef struct {
	const char* keyword;
	int token;
} Keyword;

Keyword keywords[] = {
	{"if", TKIF},
	{"for", TKFOR},
	{"while", TKWHILE},
	{"return", TKRETURN},
	{"else", TKELSE},
	{"do", TKDO},
	{"void", TKVOID},
	{"char", TKCHARKW},
	{"int", TKINTKW},
	{"string", TKSTRINGKW},
	{"bool", TKBOOLKW},
	{"true", TKBOOL},
	{"false", TKBOOL}
};
static const int keywordCount = (int)(sizeof(keywords) / sizeof(keywords[0]));
extern "C" int yylex();
extern char *yytext;
FILE* out = NULL;

int linecount = 1;

void Addline(const char* text)
{
	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] == '\n') {
			linecount++;
		}
	}
}

int line_num(){
	return linecount;
}
int checkKeyword(const char* word){
	for (int i = 0; i < keywordCount; i++) {
        if (strcmp(word, keywords[i].keyword) == 0) {
            return keywords[i].token;
        }
    }
    return TKVARIABLE;
}
%}

%%
\n+       {   Addline(yytext);    }

[ \t\f]+		{	/* ignore */	}

"//".*			{	/* ignore */	}

"/*"([^*]|\*+[^*/])*\*+\/ 		{	/* ignore */	}

"++"|"--"|"<="|">="|"||"|"&&"|"=="|"!="|">>"|"<<"|"::"|"->"|"+="|"*="|"-="|"/="|"%="		{	fprintf(out, "%d ", TKOPERATOR); return TKOPERATOR;	}

[+\-^~/%*<>?&|=!]		{	
							fprintf(out, "%d ", TKOPERATOR);
							return TKOPERATOR;	
						}

[(){}\[\];,.:]			{	
							fprintf(out, "%d ", TKPUNC);
							return TKPUNC;		
						}

\*[A-Za-z_][A-Za-z0-9_]*		{	
									fprintf(out, "%d ", TKPOINTER);
									return TKPOINTER;	
								}

[a-zA-Z_][a-zA-Z0-9_]*	{	
							int token = checkKeyword(yytext);
							fprintf(out, "%d ", token);
							return token;	
						}

\"([^\"\\]|\\.)*\"          { 
								fprintf(out, "%d ", TKSTRING);
								return TKSTRING; 
							}

[0-9]+(\.[0-9]+)?           { 
								fprintf(out, "%d ", TKNUM);
								return TKNUM; 
							}

\'([^\'\\]|\\.)\'         	{	
								fprintf(out, "%d ", TKCHAR);
								return TKCHAR; 		
							}

\#[^\n]*     { /* ignore */ }

.				{   
					fprintf(out, "%d ", TKUNMATCHED);
					return TKUNMATCHED;   
				}

%%
int yywrap(void){
	return 1;
}

int main(){
	out = fopen("scanner_out.txt", "w");
	if(!out){
		perror("scanner_out.txt");
        return 1;
	}
	int token;
	token = yylex();
	while(token){
		token = yylex();
	}

	fprintf(out, "\n");
	fclose(out);
    return 0;
}
