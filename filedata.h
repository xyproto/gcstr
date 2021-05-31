#pragma once

#include "structs.h"

FileData* NewFileData(const String* filename);
FileData* NewFileDataCharPtr(const char* filename);
const char* FileDataToCharPtr(FileData* fd);
const String* FileDataToString(FileData* fd);
const Error* FileDataToError(FileData* fd);
uint FileDataLen(FileData* fd);
StringList* FileDataLines(FileData* fd);
