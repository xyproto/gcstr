#pragma once

#include "gotypes.h"
#include "structs.h"

void Print(const String* s);
void Println(const String* s);
void Printf1(const String* fmt, const String* a);
void Printf2(const String* fmt, const String* a, const String* b);
void Printfu(const String* fmt, uint u);
void Flush();

void PrintStringElement(uint i, const String* s);
void PrintStringList(StringList* sl);
