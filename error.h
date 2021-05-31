#pragma once

#include "structs.h"

// NewError creates a new Error struct, containing the given message
const Error* NewError(const String* message);

// MustNewError creates a new Error struct, containing the given message
const Error* MustNewError(const String* message);

// NewErrorCharPtr creates a new Error struct, containing the given message
const Error* NewErrorCharPtr(const char* message);

// ErrorToString returns the message in the given Error struct, as a String*
const String* ErrorToString(const Error* err);

// ErrorToCharPtr returns the message in the given Error struct, as a
// const char* string
const char* ErrorToCharPtr(const Error* err);

// Errorf takes a format string that may contain one "%s" marker, and a string
// that will be used in place of the "%s" marker. Returns a new Error struct
// that contains the resulting string.
const Error* Errorf(const String* fmt, const String* s);

// Errorf2 takes a format string that may contain two "%s" markers, and two
// strings that will be used in place of the "%s" markers. Returns a new
// Error struct that contains the resulting string.
const Error* Errorf2(const String* fmt, const String* a, const String* b);

// ErrorfUint takes a format string that may contain one "%u" marker, and a uint
// value that will be used in place of the "%u" marker.  Returns a new Error
// struct that contains the resulting string.
const Error* ErrorfUint(const String* fmt, uint u);

// ErrorfCharPtr takes a format string that may contain one "%s" marker,
// and a string that will be used in place of the "%s" marker.  Returns a new
// Error struct that contains the resulting string.
const Error* ErrorfCharPtr(const char* fmt, const char* s);

// Errorf2CharPtr takes a format string that may contain two "%s" markers,
// and two strings that will be used in place of the "%s" markers.  Returns
// a new Error struct that contains the resulting string.
const Error* Errorf2CharPtr(const char* fmt, const char* a, const char* b);

// ErrorfUintCharPtr takes a format string that may contain one "%u" marker,
// and a uint value that will be used in place of the "%u" marker.  Returns
// a new Error struct that contains the resulting string.
const Error* ErrorfUintCharPtr(const char* fmt, uint u);
