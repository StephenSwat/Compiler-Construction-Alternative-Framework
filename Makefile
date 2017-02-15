CC           := gcc
CFLAGS       := -Wall -std=gnu11 -Og -g -D_GNU_SOURCE -MMD
SOURCE_DIRS   = src/core/ src/phases/ src/scanparse/ src/ast/ src/backends/
SRC           = $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)*.c))
PARSER        = src/scanparse/civic.parser.c
LEXER         = src/scanparse/civic.lexer.c

.PHONY: clean test

civicc: $(PARSER:.c=.o) $(LEXER:.c=.o) $(SRC:.c=.o)
	@echo "Linking executable: $@"
	@mkdir -p bin/
	@$(CC) -o bin/$@ $(PARSER:.c=.o) $(LEXER:.c=.o) $(SRC:.c=.o)

clean:
	@rm -f bin/civicc $(SRC:.c=.o) $(SRC:.c=.d) $(LEXER) $(LEXER:.c=.o) $(LEXER:.c=.d) \
		$(PARSER) $(PARSER:.c=.output) $(PARSER:.c=.o) $(PARSER:.c=.h) $(PARSER:.c=.d)

%.o: %.c
	@echo "Compiling source code: $(notdir $@)"
	@$(CC) $(CFLAGS) $(patsubst %,-I%,$(SOURCE_DIRS)) -o $@ -c $<

%.lexer.c: $(LEXER:.lexer.c=.l)
	@echo "Generating source code from Lex specification: $(notdir $@)"
	@flex -o $@ $<

%.parser.c: $(PARSER:.parser.c=.y)
	@echo "Generating source code from Yacc specification: $(notdir $@)"
	@bison -dv -o $@ $<

test: civicc
	@test/test.sh

-include $(SRC:.c=.d)
