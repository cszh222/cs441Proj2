#include <stdbool.h>
#include "symbol_table.h"

typedef enum { typeInt, typeFloat, typeId, typeOpr} nodeEnum; /* added */
typedef enum {intValType, floatValType, boolValType, none} valEnum;
/* constants */
typedef struct {
    int value;                  /* value of constant */
} conNodeType;

/* floats - added */
typedef struct {
    double value;                /* value of double */
} floatNodeType;

/* identifiers */
typedef struct {
    symbol_entry* i;                    /* variable name - changed JWJ */
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];  /* operands (expandable) */
} oprNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    /* union must be last entry in nodeType */
    /* because operNodeType may dynamically increase */
    union {
        conNodeType con;        /* constants */
        floatNodeType fl;       /* floats */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;

typedef struct value {
    valEnum type;
    bool boolVal;
    int intVal;
    double floatVal;
}value;


#define TYPE_INT   5
#define TYPE_FLOAT 6
#define TYPE_PROC  7

/* removed extern int sym[26]; JWJ */
