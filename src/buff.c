#include <stdlib.h>

#include "../include/buff.h"

Buffer *buffCreate(void){
    Buffer *buff = malloc(sizeof(Buffer));

    buff->buffSize = BASE_BUFF_SIZE;
    buff->buff = malloc(sizeof(char) * buff->buffSize);

    return buff;
}

void buffEnsureSize(Buffer *buff, size_t needed){
    if(buff->buffSize < needed){
        for(; buff->buffSize < needed; buff->buffSize *= 2);
        buff->buff = realloc(buff->buff, buff->buffSize);
    }
}

void buffDestroy(Buffer *buff){
    if(buff->buffSize > 0){
        free(buff->buff);
    }
    buff->buffSize = 0;

    free(buff);
}