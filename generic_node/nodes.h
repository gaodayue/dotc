#ifndef NODES_H
#define NODES_H

typedef enum NodeTag
{
    T_Invalid = 0,
    
    T_SelectStmt,
    T_InsertStmt,
    T_DeleteStmt,

} NodeTag;

typedef struct Node
{
    NodeTag type;
} Node;

#define nodeTag(nodeptr) (((Node *)nodeptr)->type)

/* macro newNode is only for makeNode macro*/
#ifdef __GNUC__
#define _newNode(size, tag) \
({  Node *_result; \
    _result = (Node *) malloc(size); \
    _result->type = (tag); \
    _result; \
})

#else
extern Node *newNodeHolder;
#define _newNode(size, tag) \
(   newNodeHolder = (Node *) malloc(size), \
    newNodeHolder->type = (tag), \
    newNodeHolder \
)
#endif /* __GNUC__ */

#define makeNode(_type_) ((_type_ *) _newNode(sizeof(_type_), T_##_type_))

#endif
