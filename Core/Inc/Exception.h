#ifndef _EXCEPTION_H
#define _EXCEPTION_H
#include <stdint.h>

typedef struct Exception* ExceptionPtr ; //ExceptionPtr
typedef struct Exception Exception ;
struct Exception{
  uint32_t errorCode;
  char* errorMessage;
};

void freeException(Exception *ex);
void throwException(uint32_t errorCode,char *errorMessage,...);


#endif // _EXCEPTION_H
