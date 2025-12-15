#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gc.h>

#include "error.h"
#include "gotypes.h"
#include "panic.h"
#include "str.h"

const uint defaultStringBufferSize = 64;

String* NewString(const char* contents)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicCharPtr("could not allocate memory in NewString\n");
    }
    s->len = strlen(contents);
    if (s->len == 0) {
        s->cap = defaultStringBufferSize;
    } else {
        s->cap = s->len * 2;
    }
    s->contents = (char*)GC_MALLOC(s->cap);
    if (s->contents == nil) {
        panicCharPtr("could not allocate memory in NewString (s->contents)\n");
    }
    strcpy(s->contents, contents);
    return s;
}

String* NewStringEmpty() { return NewStringCap(defaultStringBufferSize); }

String* NewStringCap(uint cap)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicCharPtr("could not allocate memory in NewStringCap\n");
    }
    ResetCap(s, cap);
    return s;
}

String* NewStringCapLen(uint cap, uint len)
{
    String* s = NewStringCap(cap);
    s->len = len;
    return s;
}

String* NewStringMaxSize(const char* contents, size_t max_size)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicCharPtr("could not allocate memory in NewStringMaxSize\n");
    }
    s->len = strnlen(contents, max_size);
    if (s->len == 0) {
        if (defaultStringBufferSize < max_size) {
            s->cap = defaultStringBufferSize;
        } else {
            s->cap = max_size;
        }
    } else {
        s->cap = s->len * 2;
    }
    s->contents = (char*)GC_MALLOC(s->cap);
    if (s->contents == nil) {
        panicCharPtr("could not allocate memory in NewStringMaxSize (s->contents)\n");
    }
    strncpy(s->contents, contents, max_size);
    return s;
}

String* NewStringNoCopy(const char* contents)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicCharPtr("could not allocate memory in NewStringNoCopy\n");
    }
    s->len = strlen(contents);
    s->cap = s->len;
    s->contents = (char*)contents;
    return s;
}

String* NewStringNoCopyMaxSize(const char* contents, size_t max_size)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicCharPtr("could not allocate memory in NewStringNoCopyMaxSize\n");
    }
    s->len = strnlen(contents, max_size);
    s->cap = s->len;
    s->contents = (char*)contents;
    return s;
}

String* NewStringFromChar(char c)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicCharPtr("could not allocate memory in NewStringFromChar\n");
    }
    s->len = 1;
    s->cap = 1;
    s->contents = (char*)GC_MALLOC(s->cap);
    if (s->contents == nil) {
        panicCharPtr("could not allocate memory in NewStringFromChar (s->contents)\n");
    }
    s->contents[0] = c;
    return s;
}

char PopChar(String* s)
{
    s->len--;
    char c = s->contents[s->len];
    s->contents[s->len] = 0; // Needed to be able to quickly return a const char*
    return c;
}

void PushChar(String* s, char c)
{
    if (s->len + 1 >= s->cap) {
        s->cap *= 2;
        if (s->cap == 0) {
            panicCharPtr("String capacity is 0 in PushChar");
        }
        s->contents = (char*)GC_REALLOC(s->contents, s->cap);
        if (s->contents == nil) {
            panicCharPtr("could not reallocate memory in PushChar");
        }
    }
    s->contents[s->len++] = c;
    s->contents[s->len] = '\0';
}

// Reset will clear the string and ensure it has a default initial capacity
void Reset(String* s) { ResetCap(s, defaultStringBufferSize); }

// ResetCap will clear the string and set its capacity
// If cap is 0, the defaultStringBufferSize will be used.
void ResetCap(String* s, uint cap)
{
    s->len = 0;
    if (cap != 0) {
        s->cap = cap;
    } else {
        s->cap = defaultStringBufferSize;
    }
    s->contents = (char*)GC_MALLOC(s->cap);
    if (s->contents == nil) {
        panicCharPtr("could not allocate memory in ResetCap (s->contents)\n");
    }
    s->contents[0] = '\0';
}

// Shift the entire string to the right or to the left
void Shift(String* s, int offset)
{
    if (offset == 0) {
        // Do nothing
        return;
    }
    int newLength = s->len + offset;
    // Going to shift to the right and need more space
    if (newLength > s->cap) {
        s->cap *= 2;
        if (s->cap == 0) {
            panicCharPtr("Can not shift a string with zero capacity");
        }
        s->contents = (char*)GC_REALLOC(s->contents, s->cap);
        if (s->contents == nil) {
            panicCharPtr("could not reallocate memory in Shift");
        }
    }
    // Nothing left, return an empty string
    if (newLength <= 0) {
        Reset(s);
        return;
    }
    if (newLength > s->len) {
        // Shift the data to the right
        // Handle overlap manually by iterating backwards.
        for (int i = s->len; i >= 0; i--) {
            s->contents[i + offset] = s->contents[i];
        }
        // Clear the gap at the beginning
        for (int i = 0; i < offset; i++) {
            s->contents[i] = 0;
        }

        s->len = newLength;
        return;
    }
    if (newLength < s->len) {
        // Shift the data to the left. offset is negative.
        // Move data to the beginning of the buffer manually.
        // -offset is the start index of the data we want to keep.
        int start = -offset;
        for (int i = 0; i < newLength; i++) {
            s->contents[i] = s->contents[start + i];
        }
        s->contents[newLength] = '\0';
        s->len = newLength;
        return;
    }
    // This place should never be reached
}

char LeftPopChar(String* s)
{
    char c = s->contents[0];
    Shift(s, -1);
    return c;
}

void LeftPushChar(String* s, char c)
{
    Shift(s, 1);
    s->contents[0] = c;
}

const char* StringToCharPtr(const String* s) { return s->contents; }

bool HasChar(const String* s, char c)
{
    // The for loop also handles empty strings
    for (int i = 0; i < s->len; i++) {
        if (s->contents[i] == c) {
            return true;
        }
    }
    return false;
}

bool HasString(const String* s, const String* a)
{
    return nil != strstr(s->contents, a->contents);
}

char LastChar(const String* s) { return s->contents[s->len - 1]; }

char FirstChar(const String* s) { return s->contents[0]; }

void RightTrim(String* s, const String* sep)
{
    while (HasChar(sep, LastChar(s))) {
        PopChar(s);
    }
}

void LeftTrim(String* s, const String* sep)
{
    while (HasChar(sep, FirstChar(s))) {
        LeftPopChar(s);
    }
}

void TrimSep(String* s, const String* sep)
{
    RightTrim(s, sep);
    LeftTrim(s, sep);
}

// Trim whitespace from the start and end of the string
void Trim(String* s) { TrimSep(s, NewString(" \n\t\r\v\f")); }

uint Len(const String* s) { return s->len; }

FindResult* Find(const String* s, const String* a)
{
    FindResult* fr = (FindResult*)GC_MALLOC(sizeof(FindResult));
    if (fr == nil) {
        panicCharPtr("could not allocate memory in Find");
    }
    fr->err = nil;
    fr->pos = 0;
    if (Len(a) == 0) {
        fr->err = NewErrorCharPtr("searching for an empty string");
        return fr;
    }
    if (Len(s) == 0) {
        fr->err = NewErrorCharPtr("searching in an empty string");
    }
    char* p = strstr(s->contents, a->contents);
    if (p == nil) {
        fr->err = Errorf(NewString("could not find %s in string"), a);
        return fr;
    }
    // Subtract two char pointers to get the char position
    fr->pos = p - s->contents;
    return fr;
}

FindResult* FindFrom(uint pos, const String* s, const String* a)
{
    FindResult* fr = (FindResult*)GC_MALLOC(sizeof(FindResult));
    if (fr == nil) {
        panicCharPtr("could not allocate memory in Find");
    }
    String* tmp = NewString(s->contents);
    if ((Len(tmp) - pos) < 0) {
        fr->err = ErrorfUintCharPtr("invalid position given to FindFrom: %u", pos);
        return fr;
    }
    // Shift the string to the left, removing the first pos characters.
    Shift(tmp, -pos);
    // Reuse fr, but this time with a search in the sub string
    fr = Find(tmp, a);
    // Add pos, to compensate for the shift
    fr->pos += pos;
    return fr;
}

uint Count(const String* s, const String* e)
{
    // printf("Looking for %s in %s.\n", e->contents, s->contents);
    uint counter = 0;
    uint next_pos = 0;
    FindResult* fr = FindFrom(next_pos, s, e);
    const Error* err = FindResultToError(fr);
    while (err == nil) {
        // Count a search that did not return an error
        counter++;
        // Update the next position to search from
        next_pos = FindResultToPos(fr) + Len(e);
        // Exit if the end is reached
        if (next_pos >= s->len) {
            return counter;
        }
        // Search again
        fr = FindFrom(next_pos, s, e);
        err = FindResultToError(fr);
    }
    return counter;
}

uint FindResultToPos(FindResult* fr)
{
    if (fr == nil) {
        panicCharPtr("FindResultToPos was given an uninitialized FindResult*");
    }
    return fr->pos;
}

const Error* FindResultToError(FindResult* fr)
{
    if (fr == nil) {
        panicCharPtr("FindResultToError was given an uninitialized FindResult*");
    }
    return fr->err;
}

const String* FindResultToString(FindResult* fr)
{
    const Error* err = FindResultToError(fr);
    if (err != nil) {
        return Combine(NewString("could not find string: "), ErrorToString(err));
    }
    return SprintfUint(NewString("found the string at position %u."), FindResultToPos(fr));
}

const String* Sprintf(const String* fmt, const String* a)
{
    // Get the length of the resulting string
    size_t buf_size = snprintf(nil, 0, fmt->contents, a->contents) + 1;
    String* s = NewStringCapLen(buf_size, buf_size - 1);
    snprintf(s->contents, buf_size, fmt->contents, a->contents);
    return s;
}

const String* Sprintf2(const String* fmt, const String* a, const String* b)
{
    // Get the length of the resulting string
    size_t buf_size = snprintf(nil, 0, fmt->contents, a->contents, b->contents) + 1;
    String* s = NewStringCapLen(buf_size, buf_size - 1);
    snprintf(s->contents, buf_size, fmt->contents, a->contents, b->contents);
    return s;
}

const String* SprintfUint(const String* fmt, uint u)
{
    // Get the length of the resulting string
    size_t buf_size = snprintf(nil, 0, fmt->contents, u) + 1;
    String* s = NewStringCapLen(buf_size, buf_size - 1);
    snprintf(s->contents, buf_size, fmt->contents, u);
    return s;
}

const String* SprintfChar(const String* fmt, char c)
{
    // Get the length of the resulting string
    size_t buf_size = snprintf(nil, 0, fmt->contents, c) + 1;
    String* s = NewStringCapLen(buf_size, buf_size - 1);
    snprintf(s->contents, buf_size, fmt->contents, c);
    return s;
}

const String* Combine(const String* a, const String* b)
{
    // TODO: Use a more efficient implementation, creating a new string of the
    // right capacity, then copying over the characters from string a and b.
    return Sprintf2(NewStringNoCopy("%s%s"), a, b);
}

// Append string b at the end of string a. Will modify string a.
void Append(String* a, const String* b)
{
    for (uint i = 0; i < Len(b); i++) {
        PushChar(a, b->contents[i]);
    }
}

// AppendChar is an alias for PushChar
void AppendChar(String* a, char b) { PushChar(a, b); }

void AppendCharPtr(String* a, const char* b) { Append(a, NewString(b)); }

// Append the result of a sprintf-like format string and a string
void Appendf(String* a, const String* fmt, const String* b) { Append(a, Sprintf(fmt, b)); }

// Append the result of a sprintf-like format string and uint
void AppendfUint(String* a, const String* fmt, uint u) { Append(a, SprintfUint(fmt, u)); }

// Append the result of a sprintf-like format string and a char
void AppendfChar(String* a, const String* fmt, char c) { Append(a, SprintfChar(fmt, c)); }

const String* Slice(const String* s, uint from, uint upto)
{
    if (from >= Len(s)) {
        panicCharPtr("can not slice from after the string");
    }
    if (upto < from) {
        panicCharPtr("when calling Slice, up to is smaller than from");
    }
    // Clamp upto if it is too high
    if (upto >= Len(s)) {
        upto = Len(s);
    }
    uint len = upto - from;
    // Generate a new string to be filled with the contents of the sliced string
    String* s2 = NewStringCap(len + 1);
    for (uint i = from; i < upto; i++) {
        // TODO: For performance, add a slice function that destroys the given string by inserting
        // a
        //       \0 when the slice should end, and moves the front pointer to the first position.
        PushChar(s2, s->contents[i]);
    }
    return s2;
}

bool Equal(const String* a, const String* b)
{
    if (a->len == 0 && b->len == 0) { // both strings are empty
        return true;
    } else if (a->len != b->len) { // different length
        return false;
    }
    return strncmp(a->contents, b->contents, a->len) == 0; // equality check
}

bool EqualCharPtr(const String* a, const char* b)
{
    if (a->len == 0 && b[0] == 0) { // both strings are empty
        return true;
    }
    return strncmp(a->contents, b, a->len) == 0; // equality check
}

bool HasPrefix(const String* s, const String* prefix)
{
    if (s->len < prefix->len) {
        return false; // the string is shorter than the prefix
    }
    for (uint i = 0; i < prefix->len; i++) {
        if (s->contents[i] != prefix->contents[i]) {
            return false; // mismatch
        }
    }
    return true;
}

bool HasPrefixCharPtr(const String* s, const char* prefix)
{
    for (uint i = 0; i < s->len; i++) {
        if (prefix[i] == '\0') {
            return true; // reached the end of the prefix, and they match
        }
        if (s->contents[i] != prefix[i]) {
            return false; // mismatch
        }
    }
    // If we reached here, s matches prefix up to s->len.
    // If prefix[s->len] is '\0', it's a match. Otherwise prefix is longer.
    return prefix[s->len] == '\0';
}

bool HasSuffix(const String* s, const String* suffix)
{
    if (s->len < suffix->len) {
        return false; // the string is shorter than the suffix
    }
    uint offset = s->len - suffix->len;
    for (uint i = 0; i < suffix->len; i++) {
        if (s->contents[offset + i] != suffix->contents[i]) {
            return false; // mismatch
        }
    }
    return true;
}

bool HasSuffixCharPtr(const String* s, const char* suffix)
{
    uint suffix_len = 0;
    while (suffix[suffix_len] != '\0') {
        suffix_len++;
    }
    if (s->len < suffix_len) {
        return false;
    }
    uint start = s->len - suffix_len;
    for (uint i = 0; i < suffix_len; i++) {
        if (s->contents[start + i] != suffix[i]) {
            return false;
        }
    }
    return true;
}

const String* ListString(const String* s)
{
    String* sb = NewStringEmpty();
    AppendCharPtr(sb, "String*");
    if (s == nil) {
        AppendCharPtr(sb, "\tuninitialized");
        return sb;
    }
    if (s->contents == nil) {
        AppendCharPtr(sb, "\tuninitialized contents");
        return sb;
    }
    if (s->len == 0) {
        AppendCharPtr(sb, "\t<empty>");
        return sb;
    }
    AppendCharPtr(sb, "\n");
    for (uint i = 0; i < Len(s) - 1; i++) {
        const char c = s->contents[i];
        switch (c) {
        case ' ':
            AppendfUint(sb, NewString("\t[%04u] <space>\n"), i);
            continue;
        case '\n':
            AppendfUint(sb, NewString("\t[%04u] <nl>\n"), i);
            continue;
        case '\t':
            AppendfUint(sb, NewString("\t[%04u] <tab>\n"), i);
            continue;
        case '\r':
            AppendfUint(sb, NewString("\t[%04u] <cr>\n"), i);
            continue;
        case '\v':
            AppendfUint(sb, NewString("\t[%04u] <vt>\n"), i);
            continue;
        case '\f':
            AppendfUint(sb, NewString("\t[%04u] <ff>\n"), i);
            continue;
        case '\0':
            AppendfUint(sb, NewString("\t[%04u] <nul>\n"), i);
            continue;
        }
        AppendfUint(sb, NewString("\t[%04u] "), i);
        AppendfChar(sb, NewString("%c\n"), c);
    }
    return sb;
}
