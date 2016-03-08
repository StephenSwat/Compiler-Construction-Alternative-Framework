CC           := gcc
CFLAGS       := -Wall -std=gnu11 -g -Og -D_GNU_SOURCE -pedantic
TARBALL      := coco.tar.gz
SOURCE_DIRS   = src/core/ src/phases/ src/backend/
GEN_DIR       = src/scanparse/
SRC           = $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)*.c))
PARSER        = $(addprefix $(GEN_DIR),civic_parser.c)
LEXER         = $(addprefix $(GEN_DIR),civic_lexer.c)
TAR_CONTENTS  = src README.md LICENSE.md NOTES.md Makefile

.PHONY: clean tarball test

civicc: $(PARSER:.c=.o) $(LEXER:.c=.o) $(SRC:.c=.o)
	@echo "Linking executable: $@"
	@mkdir -p bin/
	@$(CC) -o bin/$@ $(PARSER:.c=.o) $(LEXER:.c=.o) $(SRC:.c=.o)

tarball: clean $(TAR_CONTENTS)
	@echo "Creating tarball: $(TARBALL)"
	@tar -czf $(TARBALL) $(TAR_CONTENTS)

clean:
	@rm -f bin/civicc $(TARBALL) $(SRC:.c=.o)
	@rm -f src/*/*.output $(PARSER) $(PARSER:.c=.o) $(PARSER:.c=.h) $(LEXER) $(LEXER:.c=.o)

%.o: %.c
	@echo "Compiling source code: $(notdir $@)"
	@$(CC) $(CFLAGS) $(YYFLAGS) $(patsubst %,-I%,$(SOURCE_DIRS)) $(patsubst %,-I%,$(GEN_DIR)) -o $@ -c $<

$(GEN_DIR)civic_lexer.c: $(GEN_DIR)civic.l
	@echo "Generating source code from LEX specification: $(notdir $@)"
	@flex -o $(LEXER) $<

$(GEN_DIR)civic_parser.c: $(GEN_DIR)civic.y
	@echo "Generating source code from YACC specification: $(notdir $@)"
	@bison -dv -o $(PARSER) $<

test: civicc
	@bash -c 'for x in $$(find -name *.cvc); do echo $$x; cccp -v1 $$x; done'
