#pragma once

#include "structs.h"

FileData* NewFileData(String* filename);
FileData* NewFileDataConstChar(const char* filename);
const char* FileDataToConstChar(FileData* fd);
String* FileDataToString(FileData* fd);
Error* FileDataToError(FileData* fd);
uint FileDataLen(FileData* fd);
StringList* FileDataLines(FileData* fd);
