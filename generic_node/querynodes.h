#ifndef QUERYNODES_H
#define QUERYNODES_H

#include "nodes.h"

typedef struct SelectStmt
{
    NodeTag type;
    char *select_sql;    
} SelectStmt;

typedef struct InsertStmt
{
    NodeTag type;
    char *insert_sql;
} InsertStmt;

typedef struct DeleteStmt
{
    NodeTag type;
    char *delete_sql;
} DeleteStmt;

#endif
