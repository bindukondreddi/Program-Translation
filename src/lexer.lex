%{
#include "token.h"
#include "lexer.h"
#include "stringtable.h"
#include "y.tab.h"
#include <string.h>
#include <stdio.h>
%}

%option noinput
%option nounput

letter  [a-zA-Z]
digit   [0-9]
id      {letter}+
number  {digit}+

%%

[ \t] 		{}

\'(.|\\\\|\\n)\' { char* tmpstr = malloc(strlen(yytext+1));
		  strncpy(tmpstr,yytext+1,strlen(yytext)-2);
		  yylval.str = intern(tmpstr);
		  free(tmpstr);
		  return TOK_CHARCONST;
		}

"//".*\n|"\n"	{linenum++;
		}

"("|")"|","|"["|"]"|"|"|"=" { yylval.str = intern(yytext);
		return yytext[0];
		}

":"		{yylval.str = intern(yytext);
		 return TOK_COLON;
		}

";"		{yylval.str = intern(yytext);
		 return TOK_SEMI;
		}

"->"		{yylval.str = intern(yytext);
		 return TOK_ARROW;
		}

"~>"		{yylval.str = intern(yytext);
		 return TOK_ACTION;
		}

"=>"		{yylval.str = intern(yytext);
		 return TOK_THEN;
		}

true|false      {yylval.str = intern(yytext);
		 return TOK_BOOL;
		}

"*"|"/"		{yylval.str = intern(yytext);
		 return TOK_MULOP;
		}

"+"|"-"		{yylval.str = intern(yytext);
		 return TOK_ADDOP;
		}

"=="|">"|"<"	{yylval.str = intern(yytext);
		 return TOK_RELOP;
		}



def     	{yylval.str = intern(yytext);
		 return TOK_DEF;
		}

end		{yylval.str = intern(yytext);
		 return TOK_END;
		}

let		{yylval.str = intern(yytext);
		 return TOK_LET;
		}

in		{yylval.str = intern(yytext);
		 return TOK_IN;
		}

head|tail	{yylval.str = intern(yytext);
	 	 return TOK_LIST_FUNC;
		}	

print|printList|produce {yylval.str = intern(yytext);
		 	 return TOK_ACTION_FUNC;
			}

readChar|readInt {yylval.str = intern(yytext);
		  return TOK_READ;
		 }

isList|isNull|isBool|isFunction|isInt|isAction	{yylval.str = intern(yytext);
		 		 	 return TOK_CHECK_FUNC;
					}

case		{yylval.str = intern(yytext);
		 return TOK_CASE;
		}

else		{yylval.str = intern(yytext);
		 return TOK_ELSE;
		}

and|or		{yylval.str = intern(yytext);
		 return TOK_LOGIC;
	        }

not		{yylval.str = intern(yytext);
		 return TOK_NOT;
	        }

{id}    	{yylval.str = intern(yytext);
	         return TOK_ID;
        	}

{number}	{yylval.ival = atoi(yytext);
		 return TOK_INTEGER;
		}

%%

/*======================================================*
 * yylex calls yywrap() when it encounters the end of   *
 * the file.  Returned value 1 tells yylex to return 0, *
 * which is yylex's way of saying 'end of file'.        *
 *======================================================*/

int yywrap()
{
  return 1;
}
