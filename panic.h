#pragma once

#include "structs.h"

void panic(String* msg);
void panicConstChar(const char* msg);
void panicWhen(String* msg, Error* err);
void panicWhenConstChar(const char* msg, Error* err);
void panicIfError(Error* err);
