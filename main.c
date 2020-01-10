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
hash_line_t* n_table;
hash_line_t* r_table;
hash_line_t* w_table;



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
void helperend() {
	pc = pc+4;
	if(pass == 2){
		printf("\n");
	}
}

int line_table_get(int symbol,hash_line_t* yay) {
	//printf("ADDRESS OF TATBLE %x",(unsigned int)yay);
    node2_t *n = yay->entries[symbol];
    if (!n)
        return -1;
    else
        return n->address;
    
}

int line_table_set(int symbol, int address,hash_line_t* yay) {
	//printf("symbol: %u",symbol);
    node2_t *n = yay->entries[symbol];
    
    if (!n) {
        n = calloc(sizeof (node2_t), 1);
       // printf("ok");
        if (!n) {
            return -1;
        }
        //printf("ok");
        (n)->symbol = symbol;
        (n)->address = symbol;
        //printf("%x",(n)->symbol);
        yay->entries[symbol] = n;
        return yay->entries[symbol]->symbol;
    } else
        return n->symbol;
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
n_table = calloc(sizeof (hash_line_t), 1);
r_table = calloc(sizeof (hash_line_t), 1);
w_table = calloc(sizeof (hash_line_t), 1);
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
  yyin = fopen(argv[1], "r");
  yylineno = 0;
  pc = 0;

  pass = 2;
  // Pass 2
  yyparse();
  fclose(yyin);              /* Close the input file */

  // Write symbol table
  if (argv[2]) {
    FILE *symfp = fopen(argv[2], "w");
	int i = 0;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
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
        //printf("START...\n");
    }

}

void file_end()
{
    // just ignore these
}

void generate_mov4_targets()
{
		char* arrayz[] = {tempLbl,tempLbl2,tempLbl3,tempLbl4};
		int x = 0;
		int last;
		int address;
		int placetosend;
		int linenum;
		char* token;
		char* symbol;
		
		for(x = 0; x < 4; x++){
			symbol = arrayz[x];
			if(symbol[0] == ';'){
				print_bin(0,5);
				
				//printf("weird. Might be an error");
			}
			else{
				if(symbol[0] == 'W' || symbol[0] == 'w' ){
					if(x == 0){
						print_bin(0x1,4); 
					}
					symbol[strlen(symbol)-1] = 0;
					address = atoi(symbol + 2);
					print_bin(address,5); 

				} else{
					token = strtok(symbol, ",");
					linenum = atoi(token + 2);
					token = strtok(NULL, ",");
					token[strlen(token)-1] = 0;
					placetosend = atoi(token);

					address = line_table_get(linenum,n_table);
					if(x == 0){
					
						if(placetosend == 0){
						  print_bin(0x2,2); 
						  last = 2;
						} else if(placetosend == 2){
						  print_bin(0x1,2); 
						  last = 1;
						} else if(placetosend == 1){
						  print_bin(0x3,2); 
						  last = 3;
						}
						print_bin(0x0,2); 
					}
					
					print_bin(address,5); 
					
				}
			}
		}
}
void generate_readtargets()
{

		char* arrayz[] = {tempLbl,tempLbl2};
		int x = 0;
		int last;
		int address;
		int placetosend;
		int linenum;
		char* token;
		char* symbol;
		
		for(x = 0; x < 2; x++){
			symbol = arrayz[x];
			if(symbol[0] == ';'){
				if (address){
					print_bin(last,1);
					print_bin(address,7);
				} else{
					print_bin(0,8);
					//printf("weird. Might be an error");
				}
			} else{
				token = strtok(symbol, ",");
				linenum = atoi(token + 1);
				token = strtok(NULL, ",");
				token[strlen(token)-1] = 0;
				placetosend = atoi(token);
		
				address = line_table_get(linenum,n_table);

				if(placetosend == 0){
				  print_bin(0x0,1); 
				  last = 0;
				} else{
				  print_bin(0x1,1); 
				  last = 1;
				}
				print_bin(address,7); 
			}
		}
}

void generate_targets(int num_targets)
{
		char* arrayz[] = {tempLbl,tempLbl2,tempLbl3,tempLbl4};
		int x = 0;
		int last;
		int address;
		int placetosend;
		int linenum;
		char* token;
		char* symbol;
		for(x = 0; x < num_targets; x++){
			symbol = arrayz[x];
			if(symbol[0] == ';'){
				print_bin(0,9);
				//printf("weird. Might be an error");
			}
			else{
				if(symbol[0] == 'W' || symbol[0] == 'w' ){
					print_bin(0x1,4); 
					symbol[strlen(symbol)-1] = 0;
					address = atoi(symbol + 2);
					print_bin(address,5); 

				} else{
					token = strtok(symbol, ",");
					linenum = atoi(token + 2);
					token = strtok(NULL, ",");

					token[strlen(token)-1] = 0;
					placetosend = atoi(token);

					address = line_table_get(linenum,n_table);

					if(placetosend == 0){
					  print_bin(0x2,2); 
					  last = 2;
					} else if(placetosend == 2){
					  print_bin(0x1,2); 
					  last = 1;
					}else if(placetosend == 1){
					  print_bin(0x3,2); 
					  last = 3;
					}
					
					print_bin(address,7); 
					
				}
			}
		}
}
void gen_add()
{
    if (pass == 2) { //gen binary
        

        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
		generate_targets(2);

    } 
    
    helperend();
}

void gen_addi(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x1F, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        generate_targets(1);   
    }
    
    helperend();
}

void gen_and()
{
    if (pass == 2) { //gen binary
        

        
        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x8,5); //xop
		generate_targets(2);

    }
    
    helperend();
}

void gen_andi(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x1F, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x8,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        generate_targets(1);
    }
    
    helperend();
}

void gen_lsl()
{
    if (pass == 2) { //gen binary
        

        
        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xC,5); //xop
        generate_targets(2);
    }
    
    helperend();
}

void gen_lsli(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x1F, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xC,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        generate_targets(1);
    }
    
   helperend();
}

void gen_asr()
{
    if (pass == 2) { //gen binary
        


        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xE,5); //xop
        generate_targets(2);
    }
    
    helperend();
}

void gen_asri(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x1F, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xE,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        generate_targets(1);
    }
    
    helperend();
}

void gen_eql()
{
    if (pass == 2) { //gen binary
        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x10,5); //xop
        generate_targets(2);
    }
    
    helperend();
}

void gen_lw(int lsid, int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x22, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(lsid,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        generate_targets(1);
    }
    
    helperend();
}

void gen_lb(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x20, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        generate_targets(1);
    }
    
    helperend();
}

void gen_sw(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x2A, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        generate_targets(1);
    }
    
    helperend();
}

void gen_sb(int imm)
{
    if (pass == 2) { //gen binary
        

        print_bin(0x28, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x0,5); //xop
        print_bin(imm, 9);//param1
        check_bounds(imm, 9);
        generate_targets(1);
    }
    
    helperend();
}

void gen_bro(int exit, int offset)
{
    // get labels
    if (pass == 2) { //gen binary        
		print_bin(0x8, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(exit,3); //exit
        print_bin(offset, 20);//param1
    }
    
    helperend();
}

void gen_label(void)
{
    // record in symbol table that tempLbl is at address "pc"
    if(pass == 1){
       /* int ret = symbol_table_set(tempLbl, pc);
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }*/
        //IN PROGRESS
    }
}





void gen_nline(int instnum)
{
	//printf(":)");
	if(pass == 1){
		//printf("NINST:%x\n",instnum);
        int ret = line_table_set(instnum, pc,n_table);
        //printf("RETRIEVAL:%x\n",line_table_get(instnum,n_table));

        /*
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }
        */
    }

}
void gen_rline(int instnum)
{
	if(pass == 1){
		//printf("RINST:%x",instnum);
        int ret = line_table_set(instnum, pc,r_table);
        //printf("RETRIEVAL:%x\n",line_table_get(instnum,r_table));
        /*
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }*/
    }

}
void gen_wline(int instnum)
{
	if(pass == 1){
	    //printf("WINST:%x",instnum);
        int ret = line_table_set(instnum, pc,w_table);
        //printf("RETRIEVAL:%x\n",line_table_get(instnum,w_table));

/*        
        if (ret < 0) {
            fprintf(stderr, "error: malloc() failure!\n");
            exit(1);
        } else if (ret != pc) {
            fprintf(stderr, "error: duplicate label %s at both 0x%x and 0x%x\n",
                    tempLbl, pc, ret);
            exit(1);
        }*/
    }

}
void gen_xor(void){
	if (pass == 2) { //gen binary
        

        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0xA,5); //xop
		generate_targets(2);

    } 
    
    helperend();
}
void gen_or(void){
	if (pass == 2) { //gen binary
        

        print_bin(0x17, 7); // opcode
        print_bin(0x0,2); //pr
        print_bin(0x9,5); //xop
		generate_targets(2);

    } 
    
    helperend();
}
void gen_write(int greg){
	if (pass == 2) { //gen binary
        
        print_bin(0x1, 1); // opcode
        print_bin(greg, 5); // opcode
    } 
    helperend();

}
void gen_read(int greg){
	if (pass == 2) { //gen binary
        print_bin(0x1, 1); // opcode
        print_bin(greg, 5); // opcode
		generate_readtargets();

    } 
    helperend();
}

void gen_mov(void){
	if (pass == 2) { //gen binary
		print_bin(0x13, 7); // opcode
        print_bin(0x0, 2); //PR
        print_bin(0x0, 5); // ??
		generate_targets(2);

    } 
    helperend();

}
void gen_mov4(void){
	if (pass == 2) { //gen binary
        print_bin(0x7, 7); // opcode
        print_bin(0,1);
        generate_mov4_targets();

    } 
    helperend();

}
void gen_genu(int constantimm){
	if (pass == 2) { //gen binary

        print_bin(0x2, 7); // opcode
        print_bin(constantimm,16); //pr
		generate_targets(1);

    } 
    helperend();
}
void gen_movf(){
	if (pass == 2) { //gen binary

        print_bin(0x13, 7); // opcode
        print_bin(0x2, 2); //PR
        print_bin(0x0, 5); // ??
		generate_targets(2);

    } 
    helperend();
}
void gen_movt(){
	if (pass == 2) { //gen binary

        print_bin(0x13, 7); // opcode
        print_bin(0x3, 2); //PR
        print_bin(0x0, 5); // ??
		generate_targets(2);

    } 
    helperend();
}
void gen_tgt(){
	if (pass == 2) { //gen binary
        print_bin(23, 7); // opcode
        print_bin(0x0, 2); //PR
        print_bin(22, 5); // ??
		generate_targets(2);

    } 
    helperend();
}
void yyerror (const char *s)  /* Called by yyparse on error */
{
  fprintf(stderr, "FATAL ERROR: line %d: %s\n", yylineno, s);
  exit(1);
}
