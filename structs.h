#pragma once

#include <stdbool.h>

#include "gc.h"

#include "gotypes.h"
#include "structs.h"

// A String is like []byte in Go. No Runes.
typedef struct {
    uint len;
    uint cap;
    char* contents;
} String;

// Error contains a message.
// For no error, a pointer to an Error struct may be nil, like in Go.
typedef struct {
    String* message;
} Error;

// FindResult is used when searching for something that has a position
typedef struct {
    uint pos;
    Error* err;
} FindResult;

// FileData contains bytes and an error that may be nil
typedef struct {
    bytes data;
    uint len;
    Error* err;
} FileData;

// StringNode is a linked list of strings
typedef struct StringNode StringNode;
struct StringNode {
    StringNode* next;
    String* value;
};

// A StringNode can be the head of a StringList
typedef StringNode StringList;
