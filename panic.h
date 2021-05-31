#pragma once

#include "structs.h"

void panic(const String* msg);
void panicError(const Error* err);
void panicCharPtr(const char* msg);
void panicWhen(const String* msg, const Error* err);
void panicWhenCharPtr(const char* msg, const Error* err);
void panicIfError(const Error* err);
