#include <stdio.h>

#include "error.h"
#include "gotypes.h"
#include "panic.h"
#include "str.h"
#include "structs.h"

#include "stringlist.h"

// NewStringList creates a new 1 element string list
StringList* NewStringList()
{
    StringList* sl = (StringList*)GC_MALLOC(sizeof(StringList));
    if (sl == nil) {
        panicCharPtr("could not allocate memory in NewStringList");
    }
    // Set an empty string value with a small initial capacity
    sl->value = NewStringEmpty();
    // Set the pointer to the next element to nil, just to be explicit about it
    sl->next = nil;
    return sl;
}

void StringListPush(StringList* sl, const String* s)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    StringList* current = LastStringListNode(sl);
    StringList* next = (StringList*)GC_MALLOC(sizeof(StringList));
    if (next == nil) {
        panicCharPtr("could not allocate memory in StringListPush");
    }
    next->value = (String*)s;
    next->next = nil;
    current->next = next;
}

const String* StringListPop(StringList* sl)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    if (sl->next == nil) {
        // We are at the only node, return this string and set self to nil
        const String* retval = sl->value;
        sl = nil;
        return retval;
    }
    StringList* almostLast = BeforeLastStringListNode(sl);
    // Get the value of the last node
    const String* retval = almostLast->next->value;
    // Disconnect the last node
    almostLast->next = nil;
    // Return the string
    return retval;
}

uint StringListLen(StringList* sl)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    uint counter = 1;
    StringNode* current = sl;
    while (current->next != nil) {
        current = current->next;
        counter++;
    }
    return counter;
}

StringList* LastStringListNode(StringList* sl)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    StringNode* current = sl;
    while (current->next != nil) {
        current = current->next;
    }
    return current;
}

StringList* BeforeLastStringListNode(StringList* sl)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    StringNode* prev = nil;
    StringNode* current = sl;
    while (current->next != nil) {
        prev = current;
        current = current->next;
    }
    return prev;
}

// Lines splits a string on the newline character. No trimming.
StringList* Lines(const String* s)
{
    StringList* sl = NewStringList();
    StringList* current = sl;
    for (uint i = 0; i < Len(s); i++) {
        if (s->contents[i] == '\n') {
            // Add a new empty string to the string list, if the current string contains something
            StringListPush(sl, NewStringEmpty());
            current = LastStringListNode(sl);
        } else {
            // Push a character to the current StringList node
            PushChar(current->value, s->contents[i]);
        }
    }
    // If the last added string is empty, remove it
    if (Len(LastString(sl)) == 0) {
        StringListPop(sl);
    }
    return sl;
}

// Split splits a string on the given separator. No trimming.
StringList* Split(const String* s, const String* sep)
{
    StringList* sl = NewStringList();
    uint next_pos = 0;
    FindResult* fr = FindFrom(next_pos, s, sep);
    const Error* err = FindResultToError(fr);
    bool first = true; // Not very elegant, but it works
    while (err == nil) {
        // Add strings from searches that did not return an error
        if (first) {
            sl->value = (String*)Slice(s, next_pos, FindResultToPos(fr));
            first = false;
        } else {
            StringListPush(sl, Slice(s, next_pos, FindResultToPos(fr)));
        }
        // Update the next position to search from
        next_pos = FindResultToPos(fr) + Len(sep);
        // Exit if the end is reached
        if (next_pos >= s->len) {
            return sl;
        }
        // Search again
        fr = FindFrom(next_pos, s, sep);
        err = FindResultToError(fr);
    }
    StringListPush(sl, Slice(s, next_pos, Len(s)));
    return sl;
}

// Join the strings in a given StringList* to a single String*, given a separator.
const String* Join(StringList* sl, const String* sep)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    String* sb = NewStringEmpty();
    StringNode* current = sl;
    while (current->next != nil) {
        Append(sb, current->value);
        Append(sb, sep);
        current = current->next;
    }
    Append(sb, current->value);
    return sb;
}

// Join the given StringList* to a single String*, given a separator.
const String* JoinCharPtr(StringList* sl, const char* sep)
{
    String* sb = NewStringEmpty();
    StringNode* current = sl;
    while (current->next != nil) {
        Append(sb, current->value);
        Append(sb, NewStringNoCopy(sep));
        current = current->next;
    }
    Append(sb, current->value);
    return sb;
}

// SplitChar will split a string on a single character. No trimming.
StringList* SplitChar(const String* s, char c)
{
    StringList* sl = NewStringList();
    StringList* current = sl;
    for (uint i = 0; i < Len(s); i++) {
        if (s->contents[i] == c) {
            // Add a new empty string to the string list, if the current string contains something
            if (Len(current->value) > 0) {
                StringListPush(sl, NewStringEmpty());
                current = LastStringListNode(sl);
            }
        } else {
            // Push a character to the current StringList node
            PushChar(current->value, s->contents[i]);
        }
    }
    // If the last added string is empty, remove it
    if (Len(LastString(sl)) == 0) {
        StringListPop(sl);
    }
    return sl;
}

// SplitCharPtr splits a string on the given const char* separator. No trimming.
StringList* SplitCharPtr(const String* s, const char* sep) { return Split(s, NewString(sep)); }

// Fields will split a string on any whitespace and trim the values.
StringList* Fields(const String* s)
{
    StringList* sl = NewStringList();
    StringList* current = sl;
    for (uint i = 0; i < Len(s); i++) {
        if (s->contents[i] == ' ' || s->contents[i] == '\n' || s->contents[i] == '\t'
            || s->contents[i] == '\r' || s->contents[i] == '\v' || s->contents[i] == '\f'
            || s->contents[i] == '\0') {
            // Add a new empty string to the string list, if the current string contains something
            if (Len(current->value) > 0) {
                StringListPush(sl, NewStringEmpty());
                current = LastStringListNode(sl);
            }
        } else {
            // Push a character to the current StringList node, so the string will stay trimmed.
            PushChar(current->value, s->contents[i]);
        }
    }
    // If the last added string is empty, remove it
    if (Len(LastString(sl)) == 0) {
        StringListPop(sl);
    }
    return sl;
}

// FirstString returns the first string in the given StringList*
const String* FirstString(StringList* sl)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    return sl->value;
}

// LastString returns the last string in the given StringList*
const String* LastString(StringList* sl)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    return LastStringListNode(sl)->value;
}

// StringListForEach will apply the given function to each index and element in
// the given StringList*.
void StringListForEach(StringList* sl, void (*f)(uint i, const const String* s))
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    StringNode* current = sl;
    uint i = 0;
    while (current->next != nil) {
        f(i++, current->value);
        current = current->next;
    }
    f(i, current->value);
}

// StringListMap will apply the given function to each element in
// the given StringList*. Unlike StringListForEach, no index is passed to the
// function.
void StringListMap(StringList* sl, void (*f)(String* s))
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }
    StringNode* current = sl;
    while (current->next != nil) {
        f(current->value);
        current = current->next;
    }
    f(current->value);
}

// StringListFilter will apply the given function to each element in
// the given StringList* and return the elements where the function returns true.
StringList* StringListFilter(StringList* sl, bool (*f)(String* s))
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }
    StringList* filteredStringList = NewStringList();
    StringNode* current = sl;
    while (current->next != nil) {
        if (f(current->value)) {
            StringListPush(filteredStringList, current->value);
        }
        current = current->next;
    }
    if (f(current->value)) {
        StringListPush(filteredStringList, current->value);
    }
    return filteredStringList;
}

// TrimAll will trim whitespace from all strings in the given StringList*
void TrimAll(StringList* sl)
{
    if (sl == nil) {
        panicCharPtr("the given StringList* must be initialized");
    }

    StringListMap(sl, Trim);
}

// StringListFromArgs will create a new StringList* from the given count and
// char* array.
StringList* StringListFromArgs(int argc, char* argv[])
{
    StringList* sl = NewStringList();
    if (argc == 0) {
        return sl;
    }
    sl->value = NewString(argv[0]);
    for (uint i = 1; i < argc; i++) {
        StringListPush(sl, NewString(argv[i]));
    }
    return sl;
}
