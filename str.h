#pragma once

#include <stdbool.h>

#include "gc.h"
#include "gotypes.h"
#include "structs.h"

String* NewString(const char* contents);
String* NewStringNoCopy(const char* contents);
String* NewStringFromChar(char c);

const char* StringToConstChar(String* s);

uint Len(String* s);
uint Count(String* s, String* e);

char PopChar(String* s);
void PushChar(String* s, char c);
char LeftPopChar(String* s);
void LeftPushChar(String* s, char c);
bool HasChar(String* s, char c);
char LastChar(String* s);
char FirstChar(String* s);
void RightTrim(String* s, String* sep);
void LeftTrim(String* s, String* sep);
void TrimSep(String* s, String* sep);
void Trim(String* s);
void Shift(String* s, int offset);

FindResult* Find(String* s, String* a);
FindResult* FindFrom(uint pos, String* s, String* a);
uint FindResultToPos(FindResult* fr);
String* FindResultToString(FindResult* fr);
Error* FindResultToError(FindResult* fr);

String* Sprintf1(String* fmt, String* a);
String* Sprintf2(String* fmt, String* a, String* b);
String* Sprintfu(String* fmt, uint u);
String* Sprintfc(String* fmt, char c);

String* Combine(String* a, String* b);

void Append(String* a, String* b);
void AppendConstChar(String* a, const char* b);
void Appendf1(String* a, String* fmt, String* b);
void Appendfu(String* a, String* fmt, uint u);
void Appendfc(String* a, String* fmt, char c);

String* Slice(String* s, uint from, uint upto);

bool Equal(String* a, String* b);
bool EqualConstChar(String* a, const char* b);

String* ListString(String* s);
