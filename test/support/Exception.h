#ifndef _EXCEPTION_H
#define _EXCEPTION_H
#include <stdint.h>
#include "unity.h"

typedef struct Exception* ExceptionPtr ; //ExceptionPtr
typedef struct Exception Exception ;
struct Exception{
  uint32_t errorCode;
  char* errorMessage;
};
void dumpExceptionString(Exception *ex,UNITY_LINE_TYPE lineNumber);
void freeException(Exception *ex);
void throwException(uint32_t errorCode,char *errorMessage,...);

#define dumpException(ex)        \
                      dumpExceptionString(ex,__LINE__)
#endif // _EXCEPTION_H
