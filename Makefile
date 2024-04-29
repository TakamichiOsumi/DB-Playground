CC	= gcc
CFLAGS	= -Wall -O0 -g

SUBDIR_MATH_PARSER = Math-Expression-Parser

SUBDIRS	= $(SUBDIR_MATH_PARSER)

DEPENDENT_LIB_PATH = -L $(CURDIR)/$(SUBDIR_MATH_PARSER) -L $(CURDIR)/$(SUBDIR_MATH_PARSER)/Stack/ -L $(CURDIR)/$(SUBDIR_MATH_PARSER)/Linked-List/
DEPENDENT_LIBS	= -ll -lstack -llinked_list -lmexpr

DB_PLAYGROUND	= exec_db_playground

COMPONENTS	= SqlCreateParserCFG.c
OBJ_COMPONENTS	= SqlCreateParserCFG.o

all: libraries $(DB_PLAYGROUND)

libraries:
	for dir in $(SUBDIRS); do make -C $$dir; done

$(OBJ_COMPONENTS):
	for src in $(COMPONENTS); do $(CC) $(CFLAGS) $$src -c; done

$(DB_PLAYGROUND): $(OBJ_COMPONENTS)
	$(CC) $(CFLAGS) $(DEPENDENT_LIB_PATH) $(DEPENDENT_LIBS) SqlParserMain.c $^ -o $(DB_PLAYGROUND)

.phony: clean

clean:
	rm -rf *.o $(DB_PLAYGROUND) $(DB_PLAYGROUND).dSYM
	for dir in $(SUBDIRS); do cd $$dir; make clean; cd ..; done
