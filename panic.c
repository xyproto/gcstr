#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "panic.h"
#include "str.h"

void panic(String* msg)
{
    fprintf(stderr, "%s\n", StringToConstChar(msg));
    exit(1);
}

void panicConstChar(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void panicWhen(String* msg, Error* err)
{
    fprintf(stderr, "Error when %s: %s\n", StringToConstChar(msg), ErrorToConstChar(err));
    exit(1);
}

void panicWhenConstChar(const char* msg, Error* err)
{
    fprintf(stderr, "Error when %s: %s\n", msg, ErrorToConstChar(err));
    exit(1);
}

void panicIfError(Error* err)
{
    if (err != nil) {
        fprintf(stderr, "Error: %s\n", ErrorToConstChar(err));
        exit(1);
    }
}
