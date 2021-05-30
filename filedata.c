#include <stdio.h>
#include <stdlib.h>

#include "gc.h"

#include "error.h"
#include "filedata.h"
#include "gotypes.h"
#include "panic.h"
#include "str.h"
#include "stringlist.h"
#include "structs.h"

FileData* NewFileData(String* filename)
{
    FileData* fd = (FileData*)GC_MALLOC(sizeof(FileData));
    if (fd == nil) {
        panicConstChar("Could not allocate memory in NewFileData");
    }

    FILE* f = fopen(StringToConstChar(filename), "rb");
    if (f == nil) {
        fd->err = Errorf1(NewString("could not open %s"), filename);
        return fd;
    }

    if (fseek(f, 0, SEEK_END) == -1) {
        fd->err = Errorf1(NewString("could not seek to the end in %s"), filename);
        fclose(f);
        return fd;
    }
    long int filePosition = ftell(f);
    if (filePosition == -1) {
        fd->err = Errorf1(NewString("could not get the file position in %s"), filename);
        fclose(f);
        return fd;
    }
    // Use the last position in the file as the file length
    fd->len = filePosition;
    if (fseek(f, 0, SEEK_SET) == -1) { // same as rewind(f);
        fd->err = Errorf1(NewString("could not rewind %s"), filename);
        fclose(f);
        return fd;
    }
    fd->data = (char*)GC_MALLOC(fd->len + 1); // add a byte for zero-termination
    if (fd->data == nil) {
        fd->err = Errorf1(NewString("could not allocate memory for reading in %s"), filename);
        fclose(f);
        return fd;
    }
    if (fread(fd->data, fd->len, 1, f) == 0) {
        fd->err = Errorf1(NewString("read 0 bytes from %s"), filename);
        fclose(f);
        return fd;
    }
    if (fclose(f) == EOF) {
        panic(Sprintf(NewString("Could not close file: %s"), filename));
    }
    // Conclude the successful read
    fd->data[fd->len] = 0; // zero terminate the string after the contents
    fd->err = nil; // no error
    return fd;
}

FileData* NewFileDataConstChar(const char* filename) { return NewFileData(NewString(filename)); }

const char* FileDataToConstChar(FileData* fd) { return fd->data; }

String* FileDataToString(FileData* fd) { return NewStringNoCopy(fd->data); }

Error* FileDataToError(FileData* fd) { return fd->err; }

uint FileDataLen(FileData* fd) { return fd->len; }

StringList* FileDataLines(FileData* fd) { return Lines(FileDataToString(fd)); }
