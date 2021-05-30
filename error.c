#include <stdio.h>
#include <stdlib.h>

#include "gc.h"

#include "error.h"
#include "panic.h"
#include "str.h"

Error* NewError(String* message)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in NewError"));
    }
    err->message = message;
    return err;
}

Error* NewErrorConstChar(const char* message)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicConstChar("could not allocate memory in NewErrorConstChar");
    }
    err->message = NewStringNoCopy(message);
    return err;
}

String* ErrorToString(Error* err) { return err->message; }

const char* ErrorToConstChar(Error* err) { return StringToConstChar(err->message); }

Error* Errorf1ConstChar(const char* fmt, const char* s)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicConstChar("could not allocate memory in Errorf1ConstChar");
    }
    err->message = Sprintf(NewStringNoCopy(fmt), NewStringNoCopy(s));
    return err;
}

Error* Errorf2ConstChar(const char* fmt, const char* a, const char* b)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicConstChar("could not allocate memory in Errorf2ConstChar");
    }
    err->message = Sprintf2(NewStringNoCopy(fmt), NewStringNoCopy(a), NewStringNoCopy(b));
    return err;
}

Error* ErrorfuConstChar(const char* fmt, uint u)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panicConstChar("could not allocate memory in ErrorfuConstChar");
    }
    err->message = SprintfUint(NewStringNoCopy(fmt), u);
    return err;
}

Error* Errorf1(String* fmt, String* s)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in Errorf1"));
    }
    err->message = Sprintf(fmt, s);
    return err;
}

Error* Errorf2(String* fmt, String* a, String* b)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in Errorf2"));
    }
    err->message = Sprintf2(fmt, a, b);
    return err;
}

Error* Errorfu(String* fmt, uint u)
{
    Error* err = (Error*)GC_MALLOC(sizeof(Error));
    if (err == nil) { // check if not allocated
        panic(NewString("could not allocate memory in Errorfu"));
    }
    err->message = SprintfUint(fmt, u);
    return err;
}
