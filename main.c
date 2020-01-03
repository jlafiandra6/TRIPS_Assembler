/* 
   Program: main.c
   Author: Jack Lafiandra / Tom Conte
   High Level:
   In pass 1, Generates labels/Instruction numbers/addresses.
   In pass 2, Printing out the assembly.
*/

#include "main.h"
#include<stdio.h>
#include <string.h>

// Globals
static int pass;
static int pc;
static hash_table_t symbol_table;
static int found_pred;
static hash_line_t n_table;
static hash_line_t s_table;
static hash_line_t w_table;
int line_table_get(int symbol,hash_line_t yay) {
    node2_t* n;
    for (n = yay.entries[symbol];
         (n && n->symbol == symbol);
         n = n->next);
    if (!n)
        return -1;
    else
        return n->address;
}
unsigned long hash_string(char *string) {
    unsigned long hash = 0;
    while (*string)
        hash += *string++ * 29;
    return hash % HASH_TABLE_SIZE;
}

int symbol_table_get(char *symbol) {
    node_t *n;
    for (n = symbol_table.entries[hash_string(symbol)];
         n && strcmp(n->symbol, symbol);
         n = n->next);
    if (!n)
        return -1;
    else
        return n->address;
}

int symbol_table_set(char *symbol, int address) {
    node_t **n = &symbol_table.entries[hash_string(symbol)];

    for (; *n && strcmp((**n).symbol, symbol); n = &(**n).next);
    if (!*n) {
        *n = calloc(sizeof (node_t), 1);
        if (!*n) {
            return -1;
        }
        (**n).symbol = symbol;
        (**n).address = address;
        return address;
    } else
        return (**n).address;
}

void print_bin(int val, int bits) {
    while (bits--)
        putchar('0' + ((val >> bits) & 1));
}



int main(int argc, char *argv[])
{
#if YYDEBUG
  yydebug = 1;
#endif
  if(argc < 2 || argc > 3){
    printf("Usage: %s <input file> [symbol file]\nOutput is to stdout.\n", argv[0]);
    exit(-1);
  }
  
  /* Open the input file */
  yyin = fopen(argv[1], "r");
  if(!yyin){
    printf("ERROR: File could not be opened\n");
    exit(-1); 
  }
  //Pass 1
  pass = 1;
  yyparse();                 /* Parse the input file */

  // do it all again! :)
  rewind(yyin);
  yylineno = 1;
  pc = 0;

  pass = 2;
  // Pass 2
  yyparse();

  // If we had any instructions, print the last one's predicate      

  fclose(yyin);              /* Close the input file */

  // Write symbol table
  if (argv[2]) {
    FILE *symfp = fopen(argv[2], "w");

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        for (node_t *n = symbol_table.entries[i]; n; n = n->next) {
            fprintf(symfp, "%s: 0x%x\n", n->symbol, n->address);
        }
    }

    fclose(symfp);
  }
}

void check_bounds(int val, int bits) {
    if (val >= (1 << bits))
        yyerror("immediate value too large");
}

/* Gen the assembly */

void set_orig() {
/*
    if (orig < pc)
        yyerror("Backtracking with .orig is not supported");
    if (orig & 0x3)
        yyerror(".orig addresses must be word-aligned");

    if (pass == 1) {
        pc = orig;
    } else if (pass == 2) {
        // Pad with 0s
        for (; pc < orig; pc += 4) {
            print_bin(0, 32);
            putchar('\n');
        }
    }*/
    if (pass == 2) {
        printf("START...\n");
    }

}

void file_end()
{
    // just ignore these
}

void gen_add()
{
    if (pass == 2) { //gen binary
        

        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop

        char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
        
        symbol = tempLbl2;
		token = strtok(symbol, ",");
		linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		placetosend = atoi(token);
   		
   		address = line_table_get(linenum,n_table);
        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 

    } 
    
    pc = pc+4;
}

void gen_addi(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x1F, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        
		char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		printf("LINENUM: %d\n",linenum);

   		int address = line_table_get(linenum,n_table);
		printf("ADDDRESS?: %d\n",address);
        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7);     
    }
    
    pc = pc+4;
}

void gen_and()
{
    if (pass == 2) { //gen binary
        

        
        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x8,5); //xop

        char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
        
        symbol = tempLbl2;
		token = strtok(symbol, ",");
		linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		placetosend = atoi(token);
   		
   		address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 

    }
    
    pc = pc+4;
}

void gen_andi(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x1F, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x8,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        
        char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
    }
    
    pc = pc+4;
}

void gen_lsl()
{
    if (pass == 2) { //gen binary
        

        
        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xC,5); //xop

        char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
        
        symbol = tempLbl2;
		token = strtok(symbol, ",");
		linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		placetosend = atoi(token);
   		
   		address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 

    }
    
    pc = pc+4;
}

void gen_lsli(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x1F, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xC,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        
        char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 

    }
    
    pc = pc+4;
}

void gen_asr()
{
    if (pass == 2) { //gen binary
        


        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xE,5); //xop

        char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
        
        symbol = tempLbl2;
		token = strtok(symbol, ",");
		linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		placetosend = atoi(token);
   		
   		address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
    }
    
    pc = pc+4;
}

void gen_asri(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x1F, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xE,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);

		char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
    }
    
    pc = pc+4;
}

void gen_eql()
{
    if (pass == 2) { //gen binary
        


        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x10,5); //xop

        char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
        
        symbol = tempLbl2;
		token = strtok(symbol, ",");
		linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		placetosend = atoi(token);
   		
   		address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
    }
    
    pc = pc+4;
}

void gen_lw(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x22, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);

		char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
    }
    
    pc = pc+4;
}

void gen_lb(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x20, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);

		char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
    }
    
    pc = pc+4;
}

void gen_sw(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x2A, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);

		char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
    }
    
    pc = pc+4;
}

void gen_sb(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x28, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
		
		char *symbol = tempLbl;
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		int placetosend = atoi(token);
   		
   		int address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print_bin(0x2,2); 
        
        } else{
          print_bin(0x3,2); 
        }
        print_bin(address,7); 
    }
    
    pc = pc+4;
}

void gen_bro(char *label)
{
    // get labels
    if (pass == 2) { //gen binary
        

        int dest = symbol_table_get(label);
        if (dest < 0) {
            fprintf(stderr, "label %s is undefined\n", label);
            exit(1);
        }
        int offset = dest - (pc + 4);
        
		print_bin(0x8, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,3); //exit
        print_bin(offset, 20);//param1
    }
    
    pc = pc+4;
}

void gen_label(void)
{
    // record in symbol table that tempLbl is at address "pc"
    if(pass == 1){
        int ret = symbol_table_set(tempLbl, pc);
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }
    }
}



int line_table_set(int symbol, int address,hash_line_t yay) {
    node2_t **n = &yay.entries[symbol];

    for (; *n && (**n).symbol == symbol; n = &(**n).next);
    if (!*n) {
        *n = calloc(sizeof (node2_t), 1);
        if (!*n) {
            return -1;
        }
        (**n).symbol = symbol;
        (**n).address = symbol;
        return symbol;
    } else
        return (**n).address;
}

void gen_nline(int instnum)
{
	//printf(":)");
	if(pass == 1){
		printf("NLINE:%d\n",instnum);
        int ret = line_table_set(instnum, pc,n_table);
        printf("RET: %d\n",ret);
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }
    }

}
void gen_sline(int instnum)
{
	if(pass == 1){
        int ret = line_table_set(instnum, pc,s_table);
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }
    }

}
void gen_wline(int instnum)
{
	if(pass == 1){
        int ret = line_table_set(instnum, pc,w_table);
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }
    }

}
/*
void gen_ninstr(void){
	if(pass == 2){
		char *symbol = tempLbl
		char * token = strtok(symbol, ",");
		int linenum = atoi(token + 1);
      	token = strtok(NULL, ",");
      	token[strlen(token)-1] = 0;
   		placetosend = atoi(token);
   		
   		address = line_table_get(linenum,n_table);

        if(placetosend == 0){
          print()
        
        } else{
        
        
        }
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }
    }
}
void gen_sinstr(void){


}
void gen_winstr(void){


}
*/
void yyerror (const char *s)  /* Called by yyparse on error */
{
  fprintf(stderr, "FATAL ERROR: line %d: %s\n", yylineno, s);
  exit(1);
}
