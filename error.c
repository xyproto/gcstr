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
        panic(NewString("could not allocate memory in NewError"));
    }
    err->message = message;
    return err;
}

const Error* MustNewError(const String* message)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in MustNewError"));
    }
    err->message = message;
    return err;
}

const Error* NewErrorCharPtr(const char* message)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicCharPtr("could not allocate memory in NewErrorCharPtr");
    }
    err->message = NewStringNoCopy(message);
    return err;
}

const String* ErrorToString(const Error* err) { return err->message; }

const char* ErrorToCharPtr(const Error* err) { return StringToCharPtr(err->message); }

const Error* ErrorfCharPtr(const char* fmt, const char* s)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicCharPtr("could not allocate memory in ErrorfCharPtr");
    }
    err->message = Sprintf(NewStringNoCopy(fmt), NewStringNoCopy(s));
    return err;
}

const Error* Errorf2CharPtr(const char* fmt, const char* a, const char* b)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicCharPtr("could not allocate memory in Errorf2CharPtr");
    }
    err->message = Sprintf2(NewStringNoCopy(fmt), NewStringNoCopy(a), NewStringNoCopy(b));
    return err;
}

const Error* ErrorfUintCharPtr(const char* fmt, uint u)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicCharPtr("could not allocate memory in ErrorfUintCharPtr");
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
