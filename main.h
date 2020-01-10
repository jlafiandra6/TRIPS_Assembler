#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"


#define HASH_TABLE_SIZE 67

typedef struct node2_t {
    int symbol;
    int address;
    struct node2_t *next;
} node2_t;

typedef struct {
    node2_t * entries[HASH_TABLE_SIZE];
} hash_line_t;

typedef struct node {
    char *symbol;
    int address;
    struct node *next;
} node_t;

typedef struct {
    node_t *entries[HASH_TABLE_SIZE];
} hash_table_t;

#ifdef YYDEBUG
extern int yydebug;
#endif
extern FILE * yyin;
extern int yylineno;

typedef enum {FALSE = 0, TRUE = 1} boolean;



char *tempWord;              /* The last symbol read by the scanner */
char *tempLbl;               /* The last label found in the parser */
char *tempLbl2;
char *tempLbl3;               /* The last label found in the parser */
char *tempLbl4;


/* Functions */
//void *malloc_(size_t size);
//void *strdup_(char *str);
void set_orig();
void file_end();
void gen_add();
void gen_addi(int imm);
void gen_and();
void gen_andi(int imm);
void gen_xor();//NEW ADDITION
void gen_or();//NEW ADDITION
void gen_lsl();
void gen_lsli(int imm);
void gen_asr();
void gen_asri(int imm);
void gen_eql();
void gen_lw(int lsid,int imm);
void gen_lb(int imm);
void gen_sw(int imm);
void gen_sb(int imm);
void gen_bro(int exit, int offset);
void gen_label(void);
void gen_nline(int instnum);
void gen_rline(int instnum);
void gen_wline(int instnum);
void gen_genu(int constantimm);//NEW ADDITION
void gen_read(int greg);//NEW ADDITION
void gen_write(int greg);//NEW ADDITION
void gen_mov();//NEW ADDITION
void gen_mov4();//NEW ADDITION
void gen_movt();//NEW ADDITION
void gen_movf();//NEW ADDITION
void gen_tgt();//NEW ADDITION


void yyerror (const char *s);
