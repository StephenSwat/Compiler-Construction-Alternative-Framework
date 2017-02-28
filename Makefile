CC           := gcc
CFLAGS       := -Wall -std=gnu11 -Og -g -D_GNU_SOURCE -MMD
SOURCE_DIRS   = src/core/ src/phases/ src/phases/backend/ src/phases/frontend/ src/ast/
SRC           = $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)*.c))
PARSER        = src/phases/frontend/civic.parser.c
LEXER         = src/phases/frontend/civic.lexer.c
GENERATED     = src/ast/node.gen.c
INDENT_FLAGS  = -bad -bap -i4 -bls -br -brf -brs -nut -ce -cs -nprs -npcs -npsl -nbap -nbad -sob
AST           = src/ast.xml

.PHONY: clean test

civicc: $(GENERATED:.c=.o) $(PARSER:.c=.o) $(LEXER:.c=.o) $(SRC:.c=.o)
	@echo "Linking executable: $@"
	@mkdir -p bin/
	@$(CC) -o bin/$@ $(GENERATED:.c=.o) $(PARSER:.c=.o) $(LEXER:.c=.o) $(SRC:.c=.o)

clean:
	@rm -f bin/civicc $(SRC:.c=.o) $(SRC:.c=.d) $(LEXER) $(LEXER:.c=.o) $(LEXER:.c=.d) \
		$(PARSER) $(PARSER:.c=.output) $(PARSER:.c=.o) $(PARSER:.c=.h) $(PARSER:.c=.d) \
		$(GENERATED) $(GENERATED:.c=.h) $(GENERATED:.c=.o) $(GENERATED:.c=.d)

%.o: %.c
	@echo "Compiling source code: $(notdir $@)"
	@$(CC) $(CFLAGS) $(patsubst %,-I%,$(SOURCE_DIRS)) -o $@ -c $<

%.lexer.c: $(LEXER:.lexer.c=.l)
	@echo "Generating source code from Lex specification: $(notdir $@)"
	@flex -o $@ $<

%.parser.c: $(PARSER:.parser.c=.y)
	@echo "Generating source code from Yacc specification: $(notdir $@)"
	@bison -dv -o $@ $<

%.gen.c: $(AST) %.gen.c.xsl
	@echo "Generating source code from AST specification: $(notdir $@)"
	@xsltproc $(addsuffix .xsl, $@) $(AST) | indent $(INDENT_FLAGS) > $@
	@xsltproc $(addsuffix .xsl, $(@:.c=.h)) $(AST) | indent $(INDENT_FLAGS) > $(@:.c=.h)

test: civicc
	@test/test.sh

-include $(SRC:.c=.d)
