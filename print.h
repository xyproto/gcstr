#pragma once

#include "gotypes.h"
#include "structs.h"

void Print(String* s);
void Println(String* s);
void Printf1(String* fmt, String* a);
void Printf2(String* fmt, String* a, String* b);
void Printfu(String* fmt, uint u);
void Flush();

void PrintStringElement(uint i, String* s);
void PrintStringList(StringList* sl);
