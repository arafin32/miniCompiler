all: minicompiler

minicompiler: lex.yy.c parser.tab.c ast.o symbol_table.o semantic.o tac.o main.o
	gcc lex.yy.c parser.tab.c ast.o symbol_table.o semantic.o tac.o main.o -o minicompiler -lfl

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

ast.o: ast.c
	gcc -c ast.c

symbol_table.o: symbol_table.c
	gcc -c symbol_table.c

semantic.o: semantic.c
	gcc -c semantic.c

tac.o: tac.c
	gcc -c tac.c

main.o: main.c
	gcc -c main.c

clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h minicompiler