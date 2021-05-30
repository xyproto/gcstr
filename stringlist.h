#pragma once

#include "gotypes.h"
#include "structs.h"

// NewStringList creates a new 1 element string list
StringList* NewStringList();

void StringListPush(StringList* sl, String* s);
String* StringListPop(StringList* sl);
uint StringListLen(StringList* sl);
StringList* LastStringListNode(StringList* sl);
StringList* BeforeLastStringListNode(StringList* sl);
StringList* Lines(String* s);
StringList* Split(String* s, String* sep);
String* Join(StringList* sl, String* sep);
String* JoinConstChar(StringList* sl, const char* sep);
StringList* SplitChar(String* s, char c);
StringList* Fields(String* s);
String* FirstString(StringList* sl);
String* LastString(StringList* sl);
void StringListForEach(StringList* sl, void (*f)(uint i, String* s));
void PrintStringElement(uint i, String* s);
void PrintStringList(StringList* sl);
void TrimStringElement(uint i, String* s);
void TrimAll(StringList* sl);
StringList* StringListFromArgs(int argc, char* argv[]);
