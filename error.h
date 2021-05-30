#pragma once

#include "structs.h"

// NewError creates a new Error struct, containing the given message
Error* NewError(String* message);

// NewErrorConstChar creates a new Error struct, containing the given message
Error* NewErrorConstChar(const char* message);

// ErrorToString returns the message in the given Error struct, as a String*
String* ErrorToString(Error* err);

// ErrorToConstChar returns the message in the given Error struct, as a
// const char* string
const char* ErrorToConstChar(Error* err);

// Errorf1 takes a format string that may contain one "%s" marker, and a string
// that will be used in place of the "%s" marker. Returns a new Error struct
// that contains the resulting string.
Error* Errorf1(String* fmt, String* s);

// Errorf2 takes a format string that may contain two "%s" markers, and two
// strings that will be used in place of the "%s" markers. Returns a new
// Error struct that contains the resulting string.
Error* Errorf2(String* fmt, String* a, String* b);

// Errorfu takes a format string that may contain one "%u" marker, and a uint
// value that will be used in place of the "%u" marker.  Returns a new Error
// struct that contains the resulting string.
Error* Errorfu(String* fmt, uint u);

// Errorf1ConstChar takes a format string that may contain one "%s" marker,
// and a string that will be used in place of the "%s" marker.  Returns a new
// Error struct that contains the resulting string.
Error* Errorf1ConstChar(const char* fmt, const char* s);

// Errorf2ConstChar takes a format string that may contain two "%s" markers,
// and two strings that will be used in place of the "%s" markers.  Returns
// a new Error struct that contains the resulting string.
Error* Errorf2ConstChar(const char* fmt, const char* a, const char* b);

// ErrorfuConstChar takes a format string that may contain one "%u" marker,
// and a uint value that will be used in place of the "%u" marker.  Returns
// a new Error struct that contains the resulting string.
Error* ErrorfuConstChar(const char* fmt, uint u);
