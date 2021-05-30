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

const uint defaultStringBufferSize = 1024;

String* NewString(const char* contents)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicConstChar("Could not allocate memory in NewString\n");
    }
    s->len = strlen(contents);
    if (s->len == 0) {
        s->cap = defaultStringBufferSize;
    } else {
        s->cap = s->len * 2;
    }
    s->contents = (char*)GC_MALLOC(s->cap);
    if (s->contents == nil) {
        panicConstChar("Could not allocate memory in NewString (s->contents)\n");
    }
    strcpy(s->contents, contents);
    return s;
}

String* NewStringNoCopy(const char* contents)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicConstChar("Could not allocate memory in NewStringNoCopy\n");
    }
    s->len = strlen(contents);
    s->cap = s->len;
    s->contents = (char*)contents;
    return s;
}

String* NewStringFromChar(char c)
{
    String* s = (String*)GC_MALLOC(sizeof(String));
    if (s == nil) {
        panicConstChar("Could not allocate memory in NewStringFromChar\n");
    }
    s->len = 1;
    s->cap = 1;
    s->contents = (char*)GC_MALLOC(s->cap);
    if (s->contents == nil) {
        panicConstChar("Could not allocate memory in NewString (s->contents)\n");
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
    if (s->len >= s->cap) {
        s->cap *= 2;
        if (s->cap == 0) {
            panicConstChar("String capacity is 0 in PushChar");
        }
        s->contents = (char*)GC_REALLOC(s->contents, s->cap);
        if (s->contents == nil) {
            panicConstChar("Could not reallocate memory in PushChar");
        }
    }
    s->contents[s->len++] = c;
    s->contents[s->len] = '\0';
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
            panicConstChar("Can not shift a string with zero capacity");
        }
        s->contents = (char*)GC_REALLOC(s->contents, s->cap);
        if (s->contents == nil) {
            panicConstChar("Could not reallocate memory in Shift");
        }
    }
    // Nothing left, return an empty string
    if (newLength <= 0) {
        s = NewString("");
        return;
    }
    if (newLength > s->len) {
        // TODO: Find a more efficient way, by checking s->cap and
        //       reallocating if needed.
        // Shift the data to the right
        for (int i = s->len; i >= 0; i--) {
            s->contents[i + offset] = s->contents[i];
        }
        s->len = newLength;
        return;
    }
    if (newLength < s->len) {
        assert(offset < 0);
        // Shift the data to the left. offset is negative. Move the start of
        // the string by discarding -offset bytes at the beginning.
        s->contents = s->contents - offset;
        s->len = newLength;
        s->contents[s->len] = 0; // move zero terminator
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

const char* StringToConstChar(String* s) { return s->contents; }

bool HasChar(String* s, char c)
{
    // The for loop also handles empty strings
    for (int i = 0; i < s->len; i++) {
        if (s->contents[i] == c) {
            return true;
        }
    }
    return false;
}

bool HasString(String* s, String* a) { return nil != strstr(s->contents, a->contents); }

char LastChar(String* s) { return s->contents[s->len - 1]; }

char FirstChar(String* s) { return s->contents[0]; }

void RightTrim(String* s, String* sep)
{
    while (HasChar(sep, LastChar(s))) {
        PopChar(s);
    }
}

void LeftTrim(String* s, String* sep)
{
    while (HasChar(sep, FirstChar(s))) {
        LeftPopChar(s);
    }
}

void TrimSep(String* s, String* sep)
{
    RightTrim(s, sep);
    LeftTrim(s, sep);
}

// Trim whitespace from the start and end of the string
void Trim(String* s) { TrimSep(s, NewString(" \n\t\r\v\f")); }

uint Len(String* s) { return s->len; }

FindResult* Find(String* s, String* a)
{
    FindResult* fr = (FindResult*)GC_MALLOC(sizeof(FindResult));
    if (fr == nil) {
        panicConstChar("could not allocate memory in Find");
    }
    fr->err = nil;
    fr->pos = 0;
    if (Len(a) == 0) {
        fr->err = NewErrorConstChar("searching for an empty string");
        return fr;
    }
    if (Len(s) == 0) {
        fr->err = NewErrorConstChar("searching in an empty string");
    }
    char* p = strstr(s->contents, a->contents);
    if (p == nil) {
        fr->err = Errorf1(NewString("could not find %s in string"), a);
        return fr;
    }
    // Subtract two char pointers to get the char position
    fr->pos = p - s->contents;
    return fr;
}

FindResult* FindFrom(uint pos, String* s, String* a)
{
    FindResult* fr = (FindResult*)GC_MALLOC(sizeof(FindResult));
    if (fr == nil) {
        panicConstChar("could not allocate memory in Find");
    }
    String* tmp = NewString(s->contents);
    if ((Len(tmp) - pos) < 0) {
        fr->err = ErrorfuConstChar("invalid position given to FindFrom: %u", pos);
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

uint Count(String* s, String* e)
{
    // printf("Looking for %s in %s.\n", e->contents, s->contents);
    uint counter = 0;
    uint next_pos = 0;
    FindResult* fr = FindFrom(next_pos, s, e);
    Error* err = FindResultToError(fr);
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
        panicConstChar("FindResultToPos was given an uninitialized FindResult*");
    }
    return fr->pos;
}

Error* FindResultToError(FindResult* fr)
{
    if (fr == nil) {
        panicConstChar("FindResultToError was given an uninitialized FindResult*");
    }
    return fr->err;
}

String* FindResultToString(FindResult* fr)
{
    Error* err = FindResultToError(fr);
    if (err != nil) {
        return Combine(NewString("Could not find string: "), ErrorToString(err));
    }
    return Sprintfu(NewString("Found the string at position %u."), FindResultToPos(fr));
}

String* Sprintf1(String* fmt, String* a)
{
    // Get the length of the resulting string
    size_t buf_size = snprintf(nil, 0, fmt->contents, a->contents) + 1;
    // Allocate just enough memory
    char* msg = (char*)GC_MALLOC(buf_size);
    if (msg == nil) {
        panicConstChar("could not allocate memory in Sprintf1");
    }
    snprintf(msg, buf_size, fmt->contents, a->contents);
    msg[buf_size - 1] = 0;
    return NewString(msg);
}

String* Sprintf2(String* fmt, String* a, String* b)
{
    // Get the length of the resulting string
    size_t buf_size = snprintf(nil, 0, fmt->contents, a->contents, b->contents) + 1;
    // Allocate just enough memory
    char* msg = (char*)GC_MALLOC(buf_size);
    if (msg == nil) {
        panicConstChar("could not allocate memory in Sprintf2");
    }
    snprintf(msg, buf_size, fmt->contents, a->contents, b->contents);
    msg[buf_size - 1] = 0;
    return NewString(msg);
}

String* Sprintfu(String* fmt, uint u)
{
    // Get the length of the resulting string
    size_t buf_size = snprintf(nil, 0, fmt->contents, u) + 1;
    // Allocate just enough memory
    char* msg = (char*)GC_MALLOC(buf_size);
    if (msg == nil) {
        panicConstChar("could not allocate memory in Sprintfu");
    }
    snprintf(msg, buf_size, fmt->contents, u);
    msg[buf_size - 1] = 0;
    return NewString(msg);
}

String* Sprintfc(String* fmt, char c)
{
    // Get the length of the resulting string
    size_t buf_size = snprintf(nil, 0, fmt->contents, c) + 1;
    // Allocate just enough memory
    char* msg = (char*)GC_MALLOC(buf_size);
    if (msg == nil) {
        panicConstChar("could not allocate memory in Sprintfc");
    }
    snprintf(msg, buf_size, fmt->contents, c);
    msg[buf_size - 1] = 0;
    return NewString(msg);
}

String* Combine(String* a, String* b)
{
    // TODO: Use a more efficient implementation, creating a new string of the
    // right capacity, then copying over the characters from string a and b.
    return Sprintf2(NewStringNoCopy("%s%s"), a, b);
}

// Append string b at the end of string a. Will modify string a.
void Append(String* a, String* b)
{
    for (uint i = 0; i < Len(b); i++) {
        PushChar(a, b->contents[i]);
    }
}

void AppendConstChar(String* a, const char* b) { Append(a, NewString(b)); }

// Append the result of a sprintf-like format string and a string
void Appendf1(String* a, String* fmt, String* b) { Append(a, Sprintf1(fmt, b)); }

// Append the result of a sprintf-like format string and uint
void Appendfu(String* a, String* fmt, uint u) { Append(a, Sprintfu(fmt, u)); }

// Append the result of a sprintf-like format string and a char
void Appendfc(String* a, String* fmt, char c) { Append(a, Sprintfc(fmt, c)); }

String* Slice(String* s, uint from, uint upto)
{
    if (from >= Len(s)) {
        panicConstChar("can not slice from after the string");
    }
    if (upto < from) {
        panicConstChar("when calling Slice, up to is smaller than from");
    }
    // Clamp upto if it is too high
    if (upto >= Len(s)) {
        upto = Len(s);
    }
    // Generate a new string to be filled with the contents of the sliced string
    String* s2 = NewString("");
    for (uint i = from; i < upto; i++) {
        // TODO: For performance, add a slice function that destroys the given string by inserting
        // a
        //       \0 when the slice should end, and moves the front pointer to the first position.
        PushChar(s2, s->contents[i]);
    }
    return s2;
}

bool Equal(String* a, String* b) { return strcmp(a->contents, b->contents) == 0; }

bool EqualConstChar(String* a, const char* b) { return strcmp(a->contents, b) == 0; }

String* ListString(String* s)
{
    String* sb = NewString("");
    AppendConstChar(sb, "String*");
    if (s == nil) {
        AppendConstChar(sb, "\tuninitialized");
        return sb;
    }
    if (s->contents == nil) {
        AppendConstChar(sb, "\tuninitialized contents");
        return sb;
    }
    if (s->len == 0) {
        AppendConstChar(sb, "\t<empty>");
        return sb;
    }
    AppendConstChar(sb, "\n");
    for (uint i = 0; i < Len(s) - 1; i++) {
        const char c = s->contents[i];
        switch (c) {
        case ' ':
            Appendfu(sb, NewString("\t[%04u] <space>\n"), i);
            continue;
        case '\n':
            Appendfu(sb, NewString("\t[%04u] <nl>\n"), i);
            continue;
        case '\t':
            Appendfu(sb, NewString("\t[%04u] <tab>\n"), i);
            continue;
        case '\r':
            Appendfu(sb, NewString("\t[%04u] <cr>\n"), i);
            continue;
        case '\v':
            Appendfu(sb, NewString("\t[%04u] <vt>\n"), i);
            continue;
        case '\f':
            Appendfu(sb, NewString("\t[%04u] <ff>\n"), i);
            continue;
        case '\0':
            Appendfu(sb, NewString("\t[%04u] <nul>\n"), i);
            continue;
        }
        Appendfu(sb, NewString("\t[%04u] "), i);
        Appendfc(sb, NewString("%c\n"), c);
    }
    return sb;
}
