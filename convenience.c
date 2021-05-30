#include "convenience.h"
#include "error.h"
#include "filedata.h"
#include "panic.h"
#include "print.h"
#include "str.h"
#include "stringlist.h"

// MustRead will try read a file and either panic or return the contents as a
// String*.
String* MustRead(String* filename)
{
    FileData* fd = NewFileData(filename);
    Error* err = FileDataToError(fd);
    if (err != nil) {
        String* situation = NewString("reading file");
        panicWhen(situation, err);
    }
    return FileDataToString(fd);
}

// MustReadConstChar will read a file and either panic or return the contents
// as a const char*.
const char* MustReadConstChar(const char* filename)
{
    FileData* fd = NewFileDataConstChar(filename);
    Error* err = FileDataToError(fd);
    if (err != nil) {
        String* situation = NewString("reading file");
        panicWhen(situation, err);
    }
    return FileDataToConstChar(fd);
}
