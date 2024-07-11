#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SqlConst.h"
#include "SqlCreate.h"
#include "Catalog.h"

#include "../B-Plus-Tree/b_plus_tree.h"

extern bpt_tree *catalog;

static void
sql_create_print(sql_create_data *create_data){
    int i;
    printf("table name : %s\n", create_data->table_name);
    printf("no of columns : %d\n", create_data->n_cols);
    for (i = 0; i < create_data->n_cols; i++){
	printf("\tcol name : %s, type : %d, primary key ? : %s\n",
	       create_data->column_data[i].col_name,
	       create_data->column_data[i].type,
	       create_data->column_data[i].is_primary_key ? "yes" : "no");
    }

    catalog_insert_new_table(NULL, NULL);
}

void
sql_create_process_query(sql_create_data *create_data){
    sql_create_print(create_data);
}

void
sql_create_data_destroy(sql_create_data *create_data){
    memset(create_data, 0, sizeof(*create_data));
}
