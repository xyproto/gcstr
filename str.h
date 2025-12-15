#pragma once

#include <stdbool.h>

#include "gc.h"
#include "gotypes.h"
#include "structs.h"

String* NewString(const char* contents);
String* NewStringCap(uint cap);
String* NewStringCapLen(uint cap, uint len);
String* NewStringEmpty();
String* NewStringMaxSize(const char* contents, size_t max_size);
String* NewStringNoCopy(const char* contents);
String* NewStringNoCopyMaxSize(const char* contents, size_t max_size);
String* NewStringFromChar(char c);

char PopChar(String* s);
void PushChar(String* s, char c);
char LeftPopChar(String* s);
void LeftPushChar(String* s, char c);
bool HasChar(const String* s, char c);
char LastChar(const String* s);
char FirstChar(const String* s);
void RightTrim(String* s, const String* sep);
void LeftTrim(String* s, const String* sep);
void TrimSep(String* s, const String* sep);
void Trim(String* s);
void Shift(String* s, int offset);
void Reset(String* s);
void ResetCap(String* s, uint cap);

const char* StringToCharPtr(const String* s);
uint Len(const String* s);
uint Count(const String* s, const String* e);

FindResult* Find(const String* s, const String* a);
FindResult* FindFrom(uint pos, const String* s, const String* a);
uint FindResultToPos(FindResult* fr);
const String* FindResultToString(FindResult* fr);
const Error* FindResultToError(FindResult* fr);

const String* Sprintf(const String* fmt, const String* a);
const String* Sprintf2(const String* fmt, const String* a, const String* b);
const String* SprintfUint(const String* fmt, uint u);
const String* SprintfChar(const String* fmt, char c);

const String* Combine(const String* a, const String* b);

void Append(String* a, const String* b);
void AppendChar(String* a, char b);
void AppendCharPtr(String* a, const char* b);
void Appendf(String* a, const String* fmt, const String* b);
void AppendfUint(String* a, const String* fmt, uint u);
void AppendfChar(String* a, const String* fmt, char c);

const String* Slice(const String* s, uint from, uint upto);

bool Equal(const String* a, const String* b);
bool EqualCharPtr(const String* a, const char* b);

bool HasPrefix(const String* s, const String* prefix);
bool HasPrefixCharPtr(const String* s, const char* prefix);
bool HasSuffix(const String* s, const String* suffix);
bool HasSuffixCharPtr(const String* s, const char* suffix);

const String* ListString(const String* s);
