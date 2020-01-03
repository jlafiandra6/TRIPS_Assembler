%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
#include "main.h"
%}

%option yylineno

%%

[aA][dD][dD]           { return ADD;  }
[aA][dD][dD][iI]       { return ADDI;  }
[aA][nN][dD]           { return AND;  }
[aA][nN][dD][iI]       { return ANDI; }
[lL][sS][lL]			{ return LSL; }
[lL][sS][lL][iI]		{ return LSLI; }
[aA][sS][rR][iI]		{ return ASRI; }
[aA][sS][rR]			{ return ASR; }
[eE][qQ][lL]			{ return EQL; }
[lL][wW]				{ return LW; }
[lL][bB]				{ return LB; }
[sS][wW]				{ return SW; }
[sS][bB]				{ return SB; }
[bB][rR][oO]	       { return BRO;  } 
[nN][oO][pP]           { return NOP;  }
"."[bB][bB][eE][gG][iI][nN] { return ORIG;  }
"."[bB][bB][eE][nN][dD]   { return END;}
","                    { return COMMA; }
":"                    { return COLON; }
"#"                    { return POUND; }
"?"					{ return QUESTION; }
"("					{ return LPAREN; }
")"					{ return RPAREN; }

[G]"["[0-9]+"]"        { yytext[strlen(yytext)-1] = 0;
						yylval.val = atoi(yytext + 2);
                         return GREG;
                       }
                       
[nN]"["[0-9]+","[0-9]+"]" { yylval.strPtr = (char *) strcpy(malloc(strlen(yytext)+1),yytext); 
                         return NINSTR; 
                       }
                       
[wW]"["[0-9]+","[0-9]+"]" { yylval.strPtr = (char *) strcpy(malloc(strlen(yytext)+1),yytext); 
                         return WINSTR; 
                       }
[sS]"["[0-9]+","[0-9]+"]" { yylval.strPtr = (char *) strcpy(malloc(strlen(yytext)+1),yytext); 
                         return SINSTR; 

                       }
                       
[nN]"["[0-9]+"]" { 
						yytext[strlen(yytext)-1] = 0;
						yylval.val = atoi(yytext + 2);
                         return NLINE;
                       }
                       
[wW]"["[0-9]+"]" { yytext[strlen(yytext)-1] = 0;
						yylval.val = atoi(yytext + 2);
                         return WLINE;
                       }
[sS]"["[0-9]+"]" { yytext[strlen(yytext)-1] = 0;
						yylval.val = atoi(yytext + 2);
                         return SLINE;
                       }
                       
("+"|"-")?([0-9]+)     { yylval.val = (int)strtol(yytext, (char**)NULL, 0);
                         return IMM;
                       }
"0x"[0-9A-Fa-f]+       { yylval.val = (int)strtol(yytext, (char**)NULL, 0);
                         return IMM;
                       }
"x"[0-9A-Fa-f]+        { tempWord = (char *)malloc((strlen(yytext)+2));
                         tempWord[0] = '0';  
                         tempWord[1] = '\0';
                         strcat(tempWord, yytext);
                         yylval.val = (int)strtol(tempWord , (char**)NULL, 0); 
                         free(tempWord);
                         tempWord = 0;
                         return IMM;
                       }
[a-z_A-Z][a-z_A-Z0-9]* { 
                         yylval.strPtr = (char *) strcpy(malloc(strlen(yytext)+1),yytext); 
                         return LABEL; 
                       }
[  \t\n]+               /* eat up whitespace */
";"[^\n]+               /* comment */
.                      {}
%%
