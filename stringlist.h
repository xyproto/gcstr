#pragma once

#include "gotypes.h"
#include "structs.h"

// NewStringList creates a new 1 element string list
StringList* NewStringList();

void StringListPush(StringList* sl, const String* s);
const String* StringListPop(StringList* sl);
uint StringListLen(StringList* sl);
StringList* LastStringListNode(StringList* sl);
StringList* BeforeLastStringListNode(StringList* sl);
StringList* Lines(const String* s);
StringList* Split(const String* s, const String* sep);
const String* Join(StringList* sl, const String* sep);
const String* JoinCharPtr(StringList* sl, const char* sep);
StringList* SplitChar(const String* s, char c);
StringList* Fields(const String* s);
const String* FirstString(StringList* sl);
const String* LastString(StringList* sl);
void StringListForEach(StringList* sl, void (*f)(uint i, const String* s));
void PrintStringElement(uint i, const String* s);
void PrintStringList(StringList* sl);
void TrimStringElement(uint i, const String* s);
void TrimAll(StringList* sl);
StringList* StringListFromArgs(int argc, char* argv[]);
