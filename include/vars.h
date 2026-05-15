#ifndef VARS_H
#define VARS_H

#include <stdlib.h>

#define MAX_VAR_NAME_LEN 32

typedef struct Var{
    char name[MAX_VAR_NAME_LEN];
    size_t offset;
} Var;

#endif