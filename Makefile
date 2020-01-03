# comment out this line if you want parser debug info 
#DEBUG=-DYYDEBUG
DEBUG= 
# comment out this line if you are using Eos else
#LINKOPTS=-L/usr/gnu/lib 
# comment out this line if you are using cygwin 
LINKOPTS=-Wall 

opt: main.o parser.tab.o lex.yy.o 
	gcc $(LINKOPTS) -O -o opt main.o parser.tab.o lex.yy.o -ll -ly 
#cygwin: main.o parser.tab.o lex.yy.o 
#	gcc $(LINKOPTS) -O -o opt main.o parser.tab.o lex.yy.o -lfl 

main.o: main.c parser.tab.h
	gcc -O -c -g $(DEBUG) main.c 

parser.tab.o parser.tab.h: parser.y
	bison -d -v parser.y 
	gcc -O -c $(DEBUG) parser.tab.c 

lex.yy.o: parser.y scanner.l 
	flex scanner.l 
	gcc -O -c lex.yy.c 

clean: 
	rm -rf opt main.o parser.tab.o parser.tab.c parser.tab.h lex.yy.o lex.yy.c *~
