#include <stdio.h>
#include <stdbool.h>

#include "Math-Expression-Parser/ExportedParser.h"

#include "SqlEnums.h"
#include "SqlCreateParserCFG.h"
#include "Core/SqlConst.h"
#include "Core/SqlCreate.h"

sql_create_data create_data;

#define CURRENT_PARSED_TOKEN \
    (lstack.main_data[lstack.stack_pointer - 1].token_val)

static bool COLSLIST();
static bool COL();
static bool DATA_TYPE();

/* CREATE TABLE <variable> ( COLSLIST ) */
bool
create_query_parser(){
    int token_code, CKP;

    CHECKPOINT(CKP);

    create_data.n_cols = 0;

    /* assert((token_code = cyylex()) == SQL_CREATE_Q); */

    do {
	if ((token_code = cyylex()) == VARIABLE &&
	    strncmp(CURRENT_PARSED_TOKEN, "create", strlen("create")) != 0)
	    break;

	if ((token_code = cyylex()) == VARIABLE &&
	    strncmp(CURRENT_PARSED_TOKEN, "table", strlen("table")) != 0)
	    break;

	if ((token_code = cyylex()) != VARIABLE)
	    break;

	/* Copy the table name */
	strncpy(create_data.table_name, CURRENT_PARSED_TOKEN,
		SQL_TABLE_NAME_MAX_SIZE);

	if ((token_code = cyylex()) != BRACKET_START)
	    break;

	if (COLSLIST() == false)
	    break;

	if ((token_code = cyylex()) != BRACKET_END)
	    break;

	if ((token_code = cyylex()) != PARSER_EOF)
	    break;

	return true;

    } while(0);

    RESTORE_CHECKPOINT(CKP);

    return false;
}

/* COLSLIST -> COL | COL, COLSLIST */
static bool
COLSLIST(void){
    int token_code, CKP;

    CHECKPOINT(CKP);

    /* COLSLIST -> COL, COLSLIST */
    do {
	if (COL() == false)
	    break;

	/*
	 * Tricky part :
	 *
	 * Currently, COL() saves the one column set
	 * of information into the create_data. Then,
	 * calling break here, RESTORE_CHECKPOINT()
	 * and COL() again (following the grammer)
	 * will parse the same column duplicately.
	 * It leads to create wrong create_data. So,
	 * avoid that situation by checking comma.
	 */
	if ((token_code = cyylex()) != COMMA){
	    yyrewind(1);
	    return true;
	}

	if (COLSLIST() == false)
	    break;

	return true;

    } while(0);

    RESTORE_CHECKPOINT(CKP);

    return false;
}

/* COL -> <variable> data_type primary key | <variable> data_type */
static bool
COL(void){
    int token_code, CKP;

    CHECKPOINT(CKP);

    memset(&create_data.column_data[create_data.n_cols],
	   0, sizeof(create_data.column_data[0]));

    /* COL -> <variable> data_type primary key */
    do {

	if ((token_code = cyylex()) != VARIABLE)
	    break;

	/* Copy the column name */
	strncpy(create_data.column_data[create_data.n_cols].col_name,
		CURRENT_PARSED_TOKEN, SQL_COLUMN_NAME_MAX_SIZE);

	if (DATA_TYPE() == false)
	    break;

	if (!((token_code = cyylex()) == VARIABLE &&
	      strncmp(CURRENT_PARSED_TOKEN, "primary", strlen("primary")) == 0))
	    break;

	if (!((token_code = cyylex()) == VARIABLE &&
	      strncmp(CURRENT_PARSED_TOKEN, "key", strlen("key")) == 0))
	    break;

	create_data.column_data[create_data.n_cols].is_primary_key = true;
	create_data.n_cols++;

	return true;

    } while(0);

    memset(&create_data.column_data[create_data.n_cols],
	   0, sizeof(create_data.column_data[0]));
    RESTORE_CHECKPOINT(CKP);

    do {
	if ((token_code = cyylex()) != VARIABLE)
	    break;

	strncpy(create_data.column_data[create_data.n_cols].col_name,
		CURRENT_PARSED_TOKEN,
		SQL_COLUMN_NAME_MAX_SIZE);

	if (DATA_TYPE() == false)
	    break;

	create_data.n_cols++;

	return true;

    } while(0);

    memset(&create_data.column_data[create_data.n_cols],
	   0, sizeof(create_data.column_data[0]));

    RESTORE_CHECKPOINT(CKP);

    return false;
}

/* Current supported data types : INT, DOUBLE, BOOLEAN */
static bool
DATA_TYPE(){
    int token_code, CKP;

    CHECKPOINT(CKP);

    token_code = cyylex();

    do {
	if (token_code != VARIABLE)
	    break;

	if (strncmp(CURRENT_PARSED_TOKEN,
		    "integer", strlen("integer")) == 0){
	    create_data.column_data[create_data.n_cols].type = INT;
	    create_data.column_data[create_data.n_cols].data_len = sizeof(int);
	    return true;
	}

	if (strncmp(CURRENT_PARSED_TOKEN,
		    "double", strlen("double"))   == 0){
	    create_data.column_data[create_data.n_cols].type = DOUBLE;
	    create_data.column_data[create_data.n_cols].data_len = sizeof(double);
	    return true;
	}

	if (strncmp(CURRENT_PARSED_TOKEN,
		    "boolean", strlen("boolean")) == 0){
	    create_data.column_data[create_data.n_cols].type = BOOLEAN;
	    create_data.column_data[create_data.n_cols].data_len = sizeof(bool);
	    return true;
	}

	RESTORE_CHECKPOINT(CKP);

	return false;

    } while(0);

    /* Notes for extesion */
    /* switch(token_code){
	case INT:
	case DOUBLE:
	    return true;
	case VARCHAR:
	    do {
	    //  Dont' overwrite the token_code. Prepare one more variable 'code'.
	        int code;
		if ((code = cyylex()) != BRACKET_START)
		    break;
		if ((code = cyylex()) != INT)
		    break;
		create_data.column_data[create_data.n_cols].type = *token_code*;
		if ((code = cyylex()) != BRACKET_END)
		    break;
		return true;
	    } while(0);

	    RESTORE_CHECKPOINT(CKP);
	    return false;
	case VARIABLE:
	    return false;
	case BOOLEAN:
	    return true;
	default:
	    return false;
    } */

    RESTORE_CHECKPOINT(CKP);

    return false;
}
