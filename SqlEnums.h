#ifndef __SQL_ENUMS__
#define __SQL_ENUMS__

/*
 * Define enum values wihout making their actual values overlapped.
 */

typedef enum sql_query_type {
    SQL_SELECT_Q = 0,
    SQL_UPDATE_Q,
    SQL_CREATE_Q,
    SQL_DELETE_Q,
    SQL_INSERT_Q,
} sql_query_type;

typedef enum sql_aggregate_function {
    SQL_SUM = SQL_INSERT_Q + 1,
    /* SQL_MAX, */
    /* SQL_MIN, */
    SQL_COUNT,
    SQL_AVG,
} sql_aggregate_function;

typedef enum sql_keywords {
    SQL_FROM = SQL_AVG + 1,
    SQL_WHERE,
    SQL_GROUP_BY,
    SQL_HAVING,
    SQL_ORDER_BY,
    SQL_LIMIT,
    SQL_PRIMARY_KEY,
    SQL_NOT_NULL,
    SQL_SELECT,
    SQL_AS,
} sql_keywords;

typedef enum sql_op {
    SQL_NOT = SQL_AS + 1,
    SQL_IN,
    SQL_LIKE,
    SQL_BETWEEN,
} sql_op;

typedef enum sql_order {
    SQL_ORDER_BY_ASC = SQL_BETWEEN + 1,
    SQL_ORDER_BY_DSC,
} sql_order;

#endif
