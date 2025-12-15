#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "convenience.h"
#include "error.h"
#include "filedata.h"
#include "panic.h"
#include "print.h"
#include "str.h"
#include "stringlist.h"

// Does the given filename look like it could mean standard in?
const bool stdinFilename(const String* filename) {
    return (EqualCharPtr(filename, "-") || EqualCharPtr(filename, "/dev/stdin") || filename->len == 0);
}

// ReadFile reads the given file into the given contents *String.
// Will read from stdin if the filename is "-", "" or "/dev/stdin".
// An Error* is returned if there are issues.
const Error* ReadFile(const String* filename, String* contents)
{
    if (stdinFilename(filename)) {
        return ReadStdin(contents);
    }
    FileData *fd = NewFileData(filename);
    if (fd == nil) {
        return NewErrorCharPtr("could not allocate memory for a new FileData struct");
    }
    if (fd->err != nil) {
        return fd->err;
    }
    contents = (String*)FileDataToString(fd);
    return nil;
}

// ReadFileCharPtr reads the given file into the given contents *String.
// An Error* is returned if there are issues.
const Error* ReadFileCharPtr(const char* filename, char* contents)
{
    FileData* fd = NewFileDataCharPtr(filename);
    if (fd == nil) {
        return NewErrorCharPtr("could not allocate memory for a new FileData struct");
    }
    if (fd->err != nil) {
        return fd->err;
    }
    contents = (char*)FileDataToCharPtr(fd);
    return nil;
}

// MustReadFile will try read a file and either panic or return the contents as a String*.
// Will read from stdin if the filename is "-".
const String* MustReadFile(const String* filename)
{
    if (stdinFilename(filename)) {
        String* s = NewString("");
        const Error* err = ReadStdin(s);
        if (err != nil) {
            String* situation = NewString("reading from stdin");
            panicWhen(situation, err);
        }
        return s;
    }
    FileData *fd = NewFileData(filename);
    const Error* err = FileDataToError(fd);
    if (err != nil) {
        String* situation = NewString("reading file");
        panicWhen(situation, err);
    }
    return FileDataToString(fd);
}

// MustReadFileCharPtr will read a file and either panic or return the contents
// as a const char*.
const char* MustReadFileCharPtr(const char* filename)
{
    FileData* fd = NewFileDataCharPtr(filename);
    const Error* err = FileDataToError(fd);
    if (err != nil) {
        String* situation = NewString("reading file");
        panicWhen(situation, err);
    }
    return FileDataToCharPtr(fd);
}

// ReadStdin reads from stdin into a String*
const Error* ReadStdin(String* contents)
{
    // This is probably not the fastest way, but it does not require unistd.h
    char c = getchar();
    while (c != EOF) {
        AppendChar(contents, c);
        c = getchar();
    }
    if (feof(stdin)) {
        return nil; // reached end of file, no problem
    }
    if (ferror(stdin)) { // other error, return the errno error message as an Error*
        return NewErrorCharPtr(strerror(errno));
    }
    return nil; // no issue
}
