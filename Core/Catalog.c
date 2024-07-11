#include <stdio.h>
#include <stdlib.h>

#include "SqlCreate.h"
#include "../B-Plus-Tree/b_plus_tree.h"

static bpt_tree *catalog;

void
init_catalog(void){
    catalog = NULL;
}

bool
catalog_insert_new_table(bpt_tree* catalog,
			 sql_create_data *create_data){
    return false;
}
