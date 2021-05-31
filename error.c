#include <stdio.h>
#include <stdlib.h>

#include "gc.h"

#include "error.h"
#include "panic.h"
#include "str.h"

const Error* NewError(const String* message)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
    	return nil; // could not allocate an Error struct
    }
    err->message = message;
    return err;
}

const Error* MustNewError(const String* message)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in NewError"));
    }
    err->message = message;
    return err;
}

const Error* NewErrorConstChar(const char* message)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicConstChar("could not allocate memory in NewErrorConstChar");
    }
    err->message = NewStringNoCopy(message);
    return err;
}

const String* ErrorToString(const Error* err) { return err->message; }

const char* ErrorToConstChar(const Error* err) { return StringToConstChar(err->message); }

const Error* ErrorfConstChar(const char* fmt, const char* s)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicConstChar("could not allocate memory in ErrorfConstChar");
    }
    err->message = Sprintf(NewStringNoCopy(fmt), NewStringNoCopy(s));
    return err;
}

const Error* Errorf2ConstChar(const char* fmt, const char* a, const char* b)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicConstChar("could not allocate memory in Errorf2ConstChar");
    }
    err->message = Sprintf2(NewStringNoCopy(fmt), NewStringNoCopy(a), NewStringNoCopy(b));
    return err;
}

const Error* ErrorfUintConstChar(const char* fmt, uint u)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicConstChar("could not allocate memory in ErrorfUintConstChar");
    }
    err->message = SprintfUint(NewStringNoCopy(fmt), u);
    return err;
}

const Error* Errorf(const String* fmt, const String* s)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in Errorf"));
    }
    err->message = Sprintf(fmt, s);
    return err;
}

const Error* Errorf2(const String* fmt, const String* a, const String* b)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in Errorf2"));
    }
    err->message = Sprintf2(fmt, a, b);
    return err;
}

const Error* ErrorfUint(const String* fmt, uint u)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in ErrorfUint"));
    }
    err->message = SprintfUint(fmt, u);
    return err;
}
