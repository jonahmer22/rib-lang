#ifndef BUFF_H
#define BUFF_H

// idk

#include <stddef.h>

typedef struct Buffer{
    char *buff;
    size_t buffSize;
} Buffer;

#define BASE_BUFF_SIZE 1024

Buffer *buffCreate(void);

void buffEnsureSize(Buffer *buff, size_t needed);

void buffDestroy(Buffer *buff);

#endif