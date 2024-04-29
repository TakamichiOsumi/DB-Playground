#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "Math-Expression-Parser/ExportedParser.h"

#include "SqlEnums.h"
#include "SqlCreateParserCFG.h"

void
process_user_SQL(void){
    char sql_buffer[512];
    int token_code, parse_state;

    while(true){

	printf("DB-Playground=# ");

	fgets(sql_buffer, sizeof(sql_buffer), stdin);

	if (sql_buffer[0] == '\n')
	    continue;

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
			printf("parse succeeded\n");
		    else
			printf("parse failed\n");
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
