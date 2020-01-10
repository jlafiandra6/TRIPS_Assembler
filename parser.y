%{
#include <stdio.h>
#include <string.h>
#include "main.h"
extern char * yytext;
int yylex();
%}



%union {
   int val;
   char * strPtr;
}

%token <val> ADD ADDI
%token <val> AND ANDI
%token <val> LSL LSLI
%token <val> ASR ASRI
%token <val> XOR
%token <val> OR
%token <val> EQL
%token <val> LW
%token <val> LB
%token <val> SW
%token <val> SB
%token <val> BRO
%token <val> NOP
%token <val> GENU
%token <val> READ
%token <val> WRITE
%token <val> MOV
%token <val> MOV4
%token <val> MOVF
%token <val> MOVT
%token <val> TGT
%token <val> ORIG
%token <val> END
%token <val> COMMA
%token <val> COLON
%token <val> POUND
%token <val> QUESTION
%token <val> LPAREN
%token <val> RPAREN
%token <val> GREG
%token <strPtr> NINSTR
%token <strPtr> WINSTR
%token <strPtr> SINSTR
%token <strPtr> SEMICOLON
%token <val> NLINE
%token <val> WLINE
%token <val> SLINE
%token <val> IMM
%token <strPtr> LABEL
%type <val> start
%type <val> insts
%type <val> inst
%type <val> label
%type <val> inscounter
%type <strPtr> sendinst
%%

start : insts;

insts : inscounter inst insts            
        | END insts
		| END  
		{ file_end();}  
		| ORIG insts                                
		{ set_orig();}            
        ;
inst :   ADD sendinst sendinst
		{ 
		tempLbl = $2;
		tempLbl2 = $3;
		gen_add(); }
		| XOR sendinst sendinst
		{ 
		tempLbl = $2;
		tempLbl2 = $3;
		gen_xor(); }
		| OR sendinst sendinst
		{ 
		tempLbl = $2;
		tempLbl2 = $3;
		gen_or(); }
        | ADDI IMM sendinst
		{ 
		tempLbl = $3;
		gen_addi($2); }
        | AND sendinst sendinst
		{ 
		tempLbl = $2;
		tempLbl2 = $3;
		gen_and(); }
        | ANDI IMM sendinst
		{ 
		tempLbl = $3;
		gen_andi($2); }
        | LSL sendinst sendinst
		{ 
		tempLbl = $2;
		tempLbl2 = $3;
		gen_lsl(); }
        | LSLI IMM sendinst
		{ 
		tempLbl = $3;
		gen_lsli($2); }
        | ASR sendinst sendinst
		{ 
			tempLbl = $2;
			tempLbl2 = $3;
			gen_asr();  }
        | ASRI IMM sendinst
		{ 
		tempLbl = $3;
		gen_asri($2);  }
        | EQL sendinst sendinst
		{ 
		tempLbl = $2;
		tempLbl2 = $3;
		gen_eql();  }
		| LW IMM IMM sendinst
		{ 
		tempLbl = $4;
		gen_lw($2,$3);  }
		| LB IMM sendinst
		{   tempLbl = $3;
			gen_lb($2);  }
		| SW IMM sendinst
		{ 
			tempLbl = $3;	
			gen_sw($2);  }
		| SB IMM sendinst
		{ tempLbl = $3;
		  gen_sb($2); }
		| BRO IMM IMM
		{ gen_bro($2,$3);}
		| GENU IMM sendinst
		{ 
		tempLbl = $3;
		gen_genu($2); }
		| READ GREG sendinst sendinst
		{
		tempLbl = $3;
		tempLbl2 = $4;
		gen_read($2);
		}
		| WRITE GREG
		{
		gen_write($2);
		}
		| MOV sendinst sendinst 
		{
		tempLbl = $2;
		tempLbl2 = $3;
		gen_mov();}
		| MOV4 sendinst sendinst sendinst sendinst
		{
		tempLbl = $2;
		tempLbl2 = $3;
		tempLbl3 = $4;
		tempLbl4 = $5;
		gen_mov4();}
		| MOVT sendinst sendinst
		{
		tempLbl = $2;
		tempLbl2 = $3;
		gen_movf();
		}
		| MOVF sendinst sendinst
		{
		tempLbl = $2;
		tempLbl2 = $3;
		gen_movf();
		}
		| TGT sendinst sendinst
		{
		tempLbl = $2;
		tempLbl2 = $3;
		gen_tgt();
		}
		;
		
label : LABEL COLON
               { tempLbl = $1;
			     gen_label(); }
	;
	
inscounter : NLINE
			 {
			  gen_nline($1);}
			 | WINSTR
			 {
			  char* text = $1;
			  text[strlen(text)-1] = 0;
			  int val = atoi(yytext + 2);

			  gen_wline(val);}
			 | SLINE
			 {
			 gen_rline($1);}
	;
sendinst : 	   NINSTR
			 | WINSTR
			 | SINSTR
			 | SEMICOLON
			 
	;	

%%
