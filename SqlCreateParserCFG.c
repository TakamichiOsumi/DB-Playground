#include <stdio.h>
#include <stdbool.h>

#include "Math-Expression-Parser/ExportedParser.h"

#include "SqlEnums.h"
#include "SqlCreateParserCFG.h"

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

	if ((token_code = cyylex()) != COMMA)
	    break;

	if (COLSLIST() == false)
	    break;

	return true;

    } while(0);

    RESTORE_CHECKPOINT(CKP);

    /* COLSLIST -> COL */
    do {
	if (COL() == false)
	    break;

	return true;
    } while(0);

    RESTORE_CHECKPOINT(CKP);

    return false;
}

/* COL -> <variable> data_type | <variable> data_type primary key */
static bool
COL(void){
    int token_code, CKP;

    CHECKPOINT(CKP);

    /* COL -> <variable> data_type primary key */
    do {

	if ((token_code = cyylex()) != VARIABLE)
	    break;

	if (DATA_TYPE() == false)
	    break;

	if ((token_code = cyylex()) != VARIABLE ||
	    strncmp(CURRENT_PARSED_TOKEN, "primary", strlen("primary")) != 0)
	    break;

	if ((token_code = cyylex()) != VARIABLE ||
	    strncmp(CURRENT_PARSED_TOKEN, "key", strlen("key")) != 0)
	    break;

	return true;

    } while(0);

    RESTORE_CHECKPOINT(CKP);

    do {
	if ((token_code = cyylex()) != VARIABLE)
	    break;

	if (DATA_TYPE() == false)
	    break;

	return true;

    } while(0);

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
		    "integer", strlen("integer")) == 0)
	    return true;

	if (strncmp(CURRENT_PARSED_TOKEN,
		    "double", strlen("double"))   == 0)
	    return true;

	if (strncmp(CURRENT_PARSED_TOKEN,
		    "boolean", strlen("boolean")) == 0)
	    return true;

	return false;

    } while(0);

    /* Notes for extesion */
    /* switch(token_code){
	case INT:
	case DOUBLE:
	    return true;
	case VARCHAR:
	    do {
		if ((token_code = cyylex()) != BRACKET_START)
		    break;
		if ((token_code = cyylex()) != INT)
		    break;
		if ((token_code = cyylex()) != BRACKET_END)
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
