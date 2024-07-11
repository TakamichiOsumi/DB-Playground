CC	= gcc
CFLAGS	= -Wall -O0 -g

SUBDIR_MATH_PARSER = Math-Expression-Parser
SUBDIR_B_PLUS_TREE = B-Plus-Tree

SUBDIRS	= $(SUBDIR_MATH_PARSER)
SUBDIRS += $(SUBDIR_B_PLUS_TREE)

DEPENDENT_LIB_PATH = -L $(CURDIR)/$(SUBDIR_MATH_PARSER)
DEPENDENT_LIB_PATH += -L $(CURDIR)/$(SUBDIR_MATH_PARSER)/Stack
DEPENDENT_LIB_PATH += -L $(CURDIR)/$(SUBDIR_MATH_PARSER)/Linked-List
DEPENDENT_LIB_PATH += -L $(CURDIR)/$(SUBDIR_B_PLUS_TREE)

DEPENDENT_LIBS	= -ll -lstack -llinked_list -lmexpr -lbplustree

DB_PLAYGROUND	= db_playground

COMPONENTS	= SqlCreateParserCFG.c Core/SqlCreate.c Core/Catalog.c
OBJ_COMPONENTS	= SqlCreateParserCFG.o SqlCreate.o Catalog.o

all: libraries $(DB_PLAYGROUND)

libraries:
	for dir in $(SUBDIRS); do make -C $$dir; done

$(OBJ_COMPONENTS):
	for src in $(COMPONENTS); do $(CC) $(CFLAGS) $$src -c; done

$(DB_PLAYGROUND): $(OBJ_COMPONENTS)
	$(CC) $(CFLAGS) $(DEPENDENT_LIB_PATH) $(DEPENDENT_LIBS) SqlParserMain.c $^ -o $(DB_PLAYGROUND)

.phony: clean

clean:
	@rm -rf *.o $(DB_PLAYGROUND)*
	@for dir in $(SUBDIRS); do cd $$dir; make clean; cd ..; done
