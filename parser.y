%{
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
%token <val> EQL
%token <val> LW
%token <val> LB
%token <val> SW
%token <val> SB
%token <val> BRO
%token <val> NOP
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

start : insts
	;
insts : insts inst            
        | inst                
        ;
inst :  label inst
		| inscounter inst
        | ORIG                                  
		{ set_orig();  }	  
        | END
		{ file_end();}
        | ADD sendinst sendinst
		{ 
		tempLbl = $2;
		tempLbl2 = $3;
		gen_add(); }
        | ADDI IMM sendinst
		{ 
		tempLbl = $3;
		//printf("%s",tempLbl);
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
		| LW IMM sendinst
		{ 
		tempLbl = $3;
		gen_lw($2);  }
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
		| BRO LABEL
		{ gen_bro($2);}
		;
		

// 
label : LABEL COLON
               { tempLbl = $1;
			     gen_label(); }
	;
	
inscounter : NLINE
			 {
			  gen_nline($1);}
			 | WLINE
			 {
			  gen_wline($1);}
			 | SLINE
			 {
			 gen_sline($1);}
	;
sendinst : 	  /* empty */
			 | NINSTR
			 | WINSTR
			 | SINSTR
	;	

%%
