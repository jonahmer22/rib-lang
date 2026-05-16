#ifndef VARS_H
#define VARS_H

#include <stdlib.h>

#define MAX_VAR_NAME_LEN 32
#define MAX_STR_LEN 1024

typedef enum{
    VAR_NUM = 0,    // floats
    VAR_STR,        // strings
    VAR_LST         // lists
} VarType;

typedef struct Var{
    char name[MAX_VAR_NAME_LEN];
    size_t offset;
    VarType type;
    char strVal[MAX_STR_LEN]; // for VAR_STR: content stored C-side, not in VM
} Var;

#endif