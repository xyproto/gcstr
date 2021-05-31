#pragma once

#include "structs.h"

FileData* NewFileData(const String* filename);
FileData* NewFileDataConstChar(const char* filename);
const char* FileDataToConstChar(FileData* fd);
const String* FileDataToString(FileData* fd);
const Error* FileDataToError(FileData* fd);
uint FileDataLen(FileData* fd);
StringList* FileDataLines(FileData* fd);
