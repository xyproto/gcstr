#pragma once

#include "structs.h"

// NewError creates a new Error struct, containing the given message
const Error* NewError(const String* message);

// MustNewError creates a new Error struct, containing the given message
const Error* MustNewError(const String* message);

// NewErrorConstChar creates a new Error struct, containing the given message
const Error* NewErrorConstChar(const char* message);

// ErrorToString returns the message in the given Error struct, as a String*
const String* ErrorToString(const Error* err);

// ErrorToConstChar returns the message in the given Error struct, as a
// const char* string
const char* ErrorToConstChar(const Error* err);

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

// ErrorfConstChar takes a format string that may contain one "%s" marker,
// and a string that will be used in place of the "%s" marker.  Returns a new
// Error struct that contains the resulting string.
const Error* ErrorfConstChar(const char* fmt, const char* s);

// Errorf2ConstChar takes a format string that may contain two "%s" markers,
// and two strings that will be used in place of the "%s" markers.  Returns
// a new Error struct that contains the resulting string.
const Error* Errorf2ConstChar(const char* fmt, const char* a, const char* b);

// ErrorfUintConstChar takes a format string that may contain one "%u" marker,
// and a uint value that will be used in place of the "%u" marker.  Returns
// a new Error struct that contains the resulting string.
const Error* ErrorfUintConstChar(const char* fmt, uint u);
