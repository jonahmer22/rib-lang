// include pre-installed cortex
#include <cortex-vm.h>

// system includes
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>


// Header includes
#include "include/buff.h"
#include "include/vars.h"

// defines
#define MAX_VARS 256
#define MAX_LEN 256

// Expression parser
uint8_t nextReg = 32;
size_t labelCount = 0;

Buffer *src = NULL;
size_t writePos = 0;

Var vars[MAX_VARS];
int varCount = 0;

static const char *ep;
static void skip(void){
	while(*ep == ' ')
		ep++;
}
static inline bool isInt(const char *c){
	return *c >= '0' && *c <= '9';
}
static inline bool isAlpha(const char *c){
	return (*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z');
}
static inline bool isAlphaNum(const char *c){
	return isAlpha(c) || isInt(c);
}

static void emitf(const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	int written = vsnprintf(src->buff + writePos, src->buffSize - writePos, fmt, args);
	va_end(args);

	if((size_t)written >= src->buffSize - writePos){
		buffEnsureSize(src, writePos + (size_t)written + 1);

		va_start(args, fmt);
		written = vsnprintf(src->buff + writePos, src->buffSize - writePos, fmt, args);
		va_end(args);
	}

	writePos += (size_t)written;
}

static uint8_t pExpr(void);
static uint8_t pStrExpr(char *out, int maxlen);

// recursive parser

static uint8_t pStrExpr(char *out, int maxlen){

}

// lowest item (parenthesis or a number)
static uint8_t pFactor(void){
	skip();
	int reg = nextReg++;

	if(isInt(ep)){
		const char *tmp = ep;
		for(; isInt(tmp); tmp++);

		if(*tmp == '.'){
			float val = strtof(ep, (char **)&ep);

			emitf("faddi t%d, zero, %f\n", reg - 32, val);
		}
		else{
			int32_t val = strtoll(ep, (char **)&ep, 0);

			emitf("addi t%d, zero, %d\n", reg - 32, val);
		}
	}
	else if(isAlpha(ep)){
		const char *tmp = ep;
		for(; isAlphaNum(tmp); tmp++);

		size_t len = tmp - ep;
		// try to fine the variable
		for(int i = 0; i < varCount; i++){
			// check if we found the variable
			if(strncmp(vars[i].name, ep, len) == 0 && vars[i].name[len] == '\0'){
				// found it

				// IMPORTANT: s0 is frame pointer; set s0 equal to sp at start
				emitf("lw t%d, s0, %d\n", reg - 32, vars[i].offset);
				break;
			}
		}
		ep = tmp;
	}
	else if(*ep == '('){
		ep++;
		reg = pExpr();
		ep++;

		nextReg--;
	}

	return reg;
}

static uint8_t pTerm(void){
	skip();
	uint8_t lhs = pFactor();
	
	while(*ep == '*' || *ep == '/'){
		skip();

		switch(*ep){
			case '*':{
				ep++;

				uint8_t next = pFactor();

				emitf("mul t%d, t%d, t%d\n", lhs - 32, lhs - 32, next - 32);
				break;
			}
			case '/':{
				ep++;

				uint8_t next = pFactor();

				emitf("div t%d, t%d, t%d\n", lhs - 32, lhs - 32, next - 32);
				break;
			}
			default:{
				fprintf(stderr, "[FATAL]: wtf.\n");
				exit(EXIT_FAILURE);
			}
		}

		nextReg--;
	}

	return lhs;
}

static uint8_t pExpr(void){
	skip();
	uint8_t lhs = pTerm();
	
	while(*ep == '+' || *ep == '-'){
		skip();

		switch(*ep){
			case '+':{
				ep++;

				uint8_t next = pTerm();

				emitf("add t%d, t%d, t%d\n", lhs - 32, lhs - 32, next - 32);
				break;
			}
			case '-':{
				ep++;

				uint8_t next = pTerm();

				emitf("sub t%d, t%d, t%d\n", lhs - 32, lhs - 32, next - 32);
				break;
			}
			default:{
				fprintf(stderr, "[FATAL]: wtf 2.0.\n");
				exit(EXIT_FAILURE);
			}
		}

		nextReg--;
	}

	return lhs;
}

static uint8_t eval(const char **p){
	ep = *p;
	skip();

	// check for assignment: name = expr
	if(isAlpha(ep)){
		const char *tmp = ep;
		for(; isAlphaNum(tmp); tmp++);

		// peek past whitespace to see if next char is '='
		const char *peek = tmp;
		while(*peek == ' ') peek++;

		if(*peek == '='){
			size_t len = tmp - ep;

			// find existing variable or register new one
			int varIdx = -1;
			for(int i = 0; i < varCount; i++){
				if(strncmp(vars[i].name, ep, len) == 0 && vars[i].name[len] == '\0'){
					varIdx = i;
					break;
				}
			}
			if(varIdx == -1){
				if(varCount >= MAX_VARS){
					fprintf(stderr, "[FATAL]: too many variables.\n");
					exit(EXIT_FAILURE);
				}
				varIdx = varCount++;

				size_t copyLen = len < MAX_VAR_NAME_LEN - 1 ? len : MAX_VAR_NAME_LEN - 1;
				memcpy(vars[varIdx].name, ep, copyLen);
				vars[varIdx].name[copyLen] = '\0';
				vars[varIdx].offset = varIdx;
			}

			ep = peek + 1; // skip past '='
			*p = ep;

			uint8_t rhs = eval(p);
			emitf("sw s0, t%d, %d\n", rhs - 32, vars[varIdx].offset);
			nextReg--;

			return rhs;
		}
	}

	// not an assignment, evaluate as expression
	uint8_t reg = pExpr();
	*p = ep;
	return reg;
}

static uint8_t evalCond(const char **p){
	// string stuff eventally goes here

	uint8_t lhs = eval(p);
	ep = *p;
	skip();
	*p = ep;

	// find out what we're doing
	const char *s = *p;
	int op = 0;
	if(s[0] == '<' && s[1] == '='){
		op = 5;
		s += 2;
	}
	else if(s[0 ]== '>' && s[1] == '='){
		op = 6; s += 2;
	}
	else if(s[0] == '<' && s[1] == '>'){
		op = 4;
		s += 2;
	}
	else if(*s == '<'){
		op=1;
		s++;
	}
	else if(*s == '>'){
		op=2;
		s++;
	}
	else if(*s == '='){
		op=3;
		s++;
	}

	*p = s;
	uint8_t rhs = eval(p);
	switch(op){
		case 1:{
			emitf("slt t%d, t%d, t%d\n", lhs - 32, lhs - 32, rhs - 32);
			break;
		}
		case 2:{
			emitf("slt t%d, t%d, t%d\n", lhs - 32, rhs - 32, lhs - 32);
			break;
		}
		case 3:{
			emitf("seq t%d, t%d, t%d\n", lhs - 32, lhs - 32, rhs - 32);
			break;
		}
		case 4:{
			emitf("seq t%d, t%d, t%d\n", lhs - 32, lhs - 32, rhs - 32);
			emitf("xori t%d, t%d, 1\n", lhs - 32, lhs - 32);
			break;
		}
		case 5:{
			emitf("slt t%d, t%d, t%d\n", lhs - 32, rhs - 32, lhs - 32);
			emitf("xori t%d, t%d, 1\n", lhs - 32, lhs - 32);
			break;
		}
		case 6:{
			emitf("slt t%d, t%d, t%d\n", lhs - 32, lhs - 32, rhs - 32);
			emitf("xori t%d, t%d, 1\n", lhs - 32, lhs - 32);
			break;
		}
		default:{
			break;
		}
	}

	nextReg--;
	skip();

	return lhs;
}

static uint8_t exec(const char *s){

}

static void run_prog(void){

}

int main(){
	puts("Reduced Instruction Basic\n");

	src = buffCreate();
	emitf("addi s0, sp, 0\n");
	
	const char *tmp = 
	"x=5+5";
	evalCond(&tmp);
	
	printf("%s\n\nis src\n", src->buff);

	CortexVM *vm = cortexVMCreate();

	cortexVMExecSource(vm, src->buff);

	int exit_c = cortexVMExecSource(vm, src->buff);

	cortexVMDestroy(vm);

	buffDestroy(src);

	return exit_c;
}