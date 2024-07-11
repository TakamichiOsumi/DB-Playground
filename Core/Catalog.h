#ifndef __CATALOG__
#define __CATALOG__

#include <stdbool.h>

#include "Catalog.h"
#include "SqlCreate.h"
#include "SqlConst.h"

#include "../B-Plus-Tree/b_plus_tree.h"
#include "../Math-Expression-Parser/MexprEnums.h"

/*
 * Table schema is collection of schema records of all columns
 * of a table.
 *
 * Table schema is modelled as simply another B+ Tree like below.
 *
 * Key : Column name (SQL_STRING)
 * Value : schema_record * object
 */
typedef struct schema_record {
    char column_name[SQL_COLUMN_NAME_MAX_SIZE];
    enum data_type dtype;
    int type_size;
    bool is_primary_key;
    int offset;
} schema_record;

void *init_catalog(void);
bool catalog_insert_new_table(bpt_tree* catalog,
			      sql_create_data *create_data);

#endif
