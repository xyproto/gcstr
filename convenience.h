#pragma once

#include "structs.h"

// ReadFile reads the given file into the given contents *String.
// An Error* is returned if there are issues.
const Error* ReadFile(const String* filename, String* contents);

// ReadFileConstChar reads the given file into the given contents *char.
// An Error* is returned if there are issues.
const Error* ReadFileConstChar(const char* filename, char* contents);

// MustReadFile takes a filename and returns the contents as a String*.
// May panic if there are issues.
const String* MustReadFile(const String* filename);

// MustReadFileConstChar takes a filename and returns the contents as a
// const char* string. May panic if there are issues.
const char* MustReadFileConstChar(const char* filename);
