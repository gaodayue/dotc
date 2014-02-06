#include <stdio.h>
#include <stdlib.h>
#include "querynodes.h"

/* hacks for newNode macro on non-gcc compiler */
Node *newNodeHolder;

int
main(int argc, char **argv)
{
    int i;

#ifdef __GNUC__
    printf("defined __GNUC__\n");
#else
    printf("not defined __GNUC__\n");
#endif

    SelectStmt *selnode = makeNode(SelectStmt);
    selnode->select_sql = "select * from people";

    InsertStmt *insnode = makeNode(InsertStmt);
    insnode->insert_sql = "insert into people values ('a', 10)";

    DeleteStmt *delnode = makeNode(DeleteStmt);
    delnode->delete_sql = "delete from people";

    Node *node_array[3] = {(Node *)selnode, (Node *)insnode, (Node *)delnode};

    for (i=0; i < 3; i++)
    {
        Node *node = node_array[i];
        switch (node->type)
        {
            case T_SelectStmt:
                printf("select sql: %s\n", ((SelectStmt *)node)->select_sql);
                break;
            case T_InsertStmt:
                printf("insert sql: %s\n", ((InsertStmt *)node)->insert_sql);
                break;
            case T_DeleteStmt:
                printf("delete sql: %s\n", ((DeleteStmt *)node)->delete_sql);
                break;
            case T_Invalid:
                printf("invalid node type\n");
                exit(1);
        }
    }

    exit(0);
    
}
