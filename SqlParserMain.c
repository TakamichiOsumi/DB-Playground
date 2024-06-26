#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "Math-Expression-Parser/ExportedParser.h"

#include "SqlEnums.h"
#include "SqlCreateParserCFG.h"

/* For create table command */
#include "Core/SqlCreate.h"
extern sql_create_data create_data;

void
process_user_SQL(void){
    char sql_buffer[512];
    int token_code, parse_state;

    while(true){

	printf("DB-Playground=# ");

	fgets(sql_buffer, sizeof(sql_buffer), stdin);

	if (sql_buffer[0] == '\n')
	    continue;

	/* Detects end-of-file. Terminate the program */
	if (feof(stdin) != 0){
	    clearerr(stdin);
	    break;
	}

	init_buffer(sql_buffer);

	/* Refer to the first token */
	token_code = cyylex();

	/* Rewind the token and execute full parse per user command */
	switch(token_code){
	    case VARIABLE:
		if (strncmp(lstack.main_data[lstack.stack_pointer - 1].token_val,
			    "create", strlen("create")) == 0){
		    yyrewind(1);
		    if ((parse_state = create_query_parser()) == true)
			sql_create_process_query(&create_data);
		    else
			printf("parse failed\n");
		    sql_create_data_destroy(&create_data);
		}else{
		    printf("Error : Unrecognized command\n");
		}
		break;
	    default:
		printf("Error : Unrecognized Input\n");
		break;
	}

	parser_stack_reset();

    } /* while */
}

int
main(int argc, char **argv){

    process_user_SQL();

    return 0;
}
