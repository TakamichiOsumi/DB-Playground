#ifndef __SQL_CREATE__
#define __SQL_CREATE__

#include <stdbool.h>
#include "SqlConst.h"

typedef struct sql_create_data {
    char table_name[SQL_TABLE_NAME_MAX_SIZE];
    int n_cols;
    struct {
	char col_name[SQL_COLUMN_NAME_MAX_SIZE];
	int type;
	int data_len;
	bool is_primary_key;
    } column_data[SQL_MAX_COLUMNS_SUPPORTED_PER_TABLE];
} sql_create_data;

void sql_create_process_query(sql_create_data *create_data);
void sql_create_data_destroy(sql_create_data *create_data);

#endif
