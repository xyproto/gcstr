#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "panic.h"
#include "str.h"

void panic(const String* msg)
{
    if (msg != nil) {
        fprintf(stderr, "%s\n", StringToCharPtr(msg));
    } else {
        fprintf(stderr, "%s\n", "error struct is nil");
    }
    exit(1);
}

void panicError(const Error* err)
{
    if (err != nil) {
        fprintf(stderr, "error: %s\n", ErrorToCharPtr(err));
    } else {
        fprintf(stderr, "%s\n", "error struct is nil");
    }
    exit(1);
}

void panicCharPtr(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void panicWhen(const String* msg, const Error* err)
{
    if (msg == nil) {
        fprintf(stderr, "%s\n", "message struct is nil");
    } else if (err == nil) {
        fprintf(stderr, "%s\n", "error struct is nil");
    } else {
        fprintf(stderr, "error when %s: %s\n", StringToCharPtr(msg), ErrorToCharPtr(err));
    }
    exit(1);
}

void panicWhenCharPtr(const char* msg, const Error* err)
{
    if (err == nil) {
        fprintf(stderr, "%s\n", "error struct is nil");
    } else {
        fprintf(stderr, "error when %s: %s\n", msg, ErrorToCharPtr(err));
    }
    exit(1);
}

void panicIfError(const Error* err)
{
    if (err != nil) {
        fprintf(stderr, "error: %s\n", ErrorToCharPtr(err));
        exit(1);
    }
}
