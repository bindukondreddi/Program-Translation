CC     = gcc
LINK   = gcc
CFLAGS = -g -Wall
RM     = rm

CFILES = src/stringtable.c\
         src/teststringtable.c\
         src/lex.yy.c\
         src/testlexer.c\
	 src/ast.c\
	 src/testast.c\
	 src/symboltable.c\
	 src/testsymbol.c\
	 src/simplify.c\
	 src/testsimp.c\
	 src/interpreter.c\
	 src/testintp.c\
	 src/y.tab.c

BFILES = src/lex.yy.c\
	src/y.tab.c

OFILES = $(patsubst src/%.c,obj/%.o,$(CFILES))

EFILES = bin/teststringtable\
         bin/testlexer\
	 bin/testparser

STRINGTABLE_OFILES = obj/stringtable.o\
                     obj/teststringtable.o

TESTLEXER_OFILES = obj/stringtable.o\
		   obj/lex.yy.o\
                   obj/testlexer.o

AST_OFILES	= obj/stringtable.o\
		  obj/ast.o\
		  obj/testast.o

SYM_OFILES	= obj/stringtable.o\
		  obj/ast.o\
		  obj/symboltable.o\
		  obj/testsymbol.o
		  
SIMP_OFILES	= obj/stringtable.o\
		  obj/ast.o\
		  obj/symboltable.o\
		  obj/simplify.o\
		  obj/testsimp.o

INTP_OFILES	= obj/stringtable.o\
		  obj/ast.o\
		  obj/symboltable.o\
		  obj/simplify.o\
		  obj/interpreter.o\
		  obj/testintp.o

PARSER_OFILES	= obj/stringtable.o\
		  obj/ast.o\
		  obj/symboltable.o\
		  obj/simplify.o\
		  obj/interpreter.o\
		  obj/y.tab.o\
		  obj/lex.yy.o

teststringtable: $(STRINGTABLE_OFILES)
	$(LINK) -o bin/teststringtable $(STRINGTABLE_OFILES)

testlexer: $(TESTLEXER_OFILES)
	$(LINK) -o bin/testlexer $(TESTLEXER_OFILES)

testast: $(AST_OFILES)
	$(LINK) -o bin/testast $(AST_OFILES)

testsymbol: $(SYM_OFILES)
	$(LINK) -o bin/testsymbol $(SYM_OFILES)
	
testsimp: $(SIMP_OFILES)
	$(LINK) -o bin/testsimp $(SIMP_OFILES)

testintp: $(INTP_OFILES)
	$(LINK) -o bin/testintp $(INTP_OFILES)

SNF: $(PARSER_OFILES)
	$(LINK) -o bin/SNF $(PARSER_OFILES)

src/lex.yy.c: src/lexer.lex
	cd src; flex lexer.lex

src/y.tab.c: src/parser.y
	cd src; bison -y -v -d parser.y

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OFILES) $(BFILES) $(EFILES)
