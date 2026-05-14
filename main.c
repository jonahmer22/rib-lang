// include pre-installed cortex
#include <cortex-vm.h>

// system includes
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Header includes
#include "include/buff.h"

// Expression parser
uint8_t nextReg = 32;
size_t labelCount = 0;

static const char *ep;
static void skip(void){
	while(*ep == ' ')
		ep++;
}
static inline bool isInt(const char *c){
    return *c >= '0' && *c <= '9';
}

static int pExpr(void);
static int pStrExpr(char *out, int maxlen);

// recursive parser

static int pStrExpr(char *out, int maxlen){

}

static int pFactor(void){

}

static int pTerm(void){

}

static int pExpr(void){

}

static int eval(const char **p){

}

static int evalCond(const char **p){

}

static int exec(const char *s){

}

int main(){

    Buffer *src = buffCreate();
    buffEnsureSize(src, sizeof(char) * 1000);

    char tmp[] =
        "addi a0, a0, 5"
        "addi a13, zero, 1"
        "syscall"
        "addi a13, zero, 0"
        "syscall";

    memcpy(src->buff, tmp, sizeof((tmp)));

    CortexVM *vm = cortexVMCreate();

    cortexVMExecSource(vm, src->buff);

    int exit_c = cortexVMExecSource(vm, src->buff);

    cortexVMDestroy(vm);

    buffDestroy(src);

    return exit_c;
}