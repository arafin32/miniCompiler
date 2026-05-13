SRC=src
BUILD=build
CC=gcc
CFLAGS=-I$(SRC) -I$(BUILD)

YACC=$(BUILD)/parser.tab.c
YACC_HDR=$(BUILD)/parser.tab.h
LEX=$(BUILD)/lex.yy.c
OBJS=$(BUILD)/ast.o $(BUILD)/symbol_table.o $(BUILD)/semantic.o $(BUILD)/tac.o $(BUILD)/optimizer.o $(BUILD)/codegen_extended.o $(BUILD)/main.o
TARGET=$(BUILD)/minicompiler

all: $(TARGET)

$(TARGET): $(LEX) $(YACC) $(OBJS)
	$(CC) $(CFLAGS) $(LEX) $(YACC) $(OBJS) -o $(TARGET) -lfl

$(YACC) $(YACC_HDR): $(SRC)/parser.y
	cd $(SRC) && CPPFLAGS=-I. bison -d -o ../$(YACC) parser.y

$(LEX): $(SRC)/lexer.l $(YACC_HDR)
	cd $(BUILD) && CPPFLAGS=-I. flex -o ../$(LEX) ../$(SRC)/lexer.l

$(BUILD)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)/*