#pragma once

#include "structs.h"

void panic(const String* msg);
void panicError(const Error* err);
void panicConstChar(const char* msg);
void panicWhen(const String* msg, const Error* err);
void panicWhenConstChar(const char* msg, const Error* err);
void panicIfError(const Error* err);
