#include "convenience.h"
#include "error.h"
#include "filedata.h"
#include "panic.h"
#include "print.h"
#include "str.h"
#include "stringlist.h"

// ReadFile reads the given file into the given contents *String.
// An Error* is returned if there are issues.
const Error* ReadFile(const String* filename, String* contents)
{
    FileData* fd = NewFileData(filename);
    if (fd == nil) {
        return NewErrorConstChar("could not allocate memory for a new FileData struct");
    }
    if (fd->err != nil) {
        return fd->err;
    }
    contents = (String*)FileDataToString(fd);
    return nil;
}

// ReadFileConstChar reads the given file into the given contents *String.
// An Error* is returned if there are issues.
const Error* ReadFileConstChar(const char* filename, char* contents)
{
    FileData* fd = NewFileDataConstChar(filename);
    if (fd == nil) {
        return NewErrorConstChar("could not allocate memory for a new FileData struct");
    }
    if (fd->err != nil) {
        return fd->err;
    }
    contents = (char*)FileDataToConstChar(fd);
    return nil;
}

// MustReadFile will try read a file and either panic or return the contents as a String*.
const String* MustReadFile(const String* filename)
{
    FileData* fd = NewFileData(filename);
    const Error* err = FileDataToError(fd);
    if (err != nil) {
        String* situation = NewString("reading file");
        panicWhen(situation, err);
    }
    return FileDataToString(fd);
}

// MustReadFileConstChar will read a file and either panic or return the contents
// as a const char*.
const char* MustReadFileConstChar(const char* filename)
{
    FileData* fd = NewFileDataConstChar(filename);
    const Error* err = FileDataToError(fd);
    if (err != nil) {
        String* situation = NewString("reading file");
        panicWhen(situation, err);
    }
    return FileDataToConstChar(fd);
}
