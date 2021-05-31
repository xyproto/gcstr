#pragma once

#include "structs.h"

// ReadFile reads the given file into the given contents *String.
// An Error* is returned if there are issues.
const Error* ReadFile(const String* filename, String* contents);

// ReadFileCharPtr reads the given file into the given contents *char.
// An Error* is returned if there are issues.
const Error* ReadFileCharPtr(const char* filename, char* contents);

// MustReadFile takes a filename and returns the contents as a String*.
// May panic if there are issues.
const String* MustReadFile(const String* filename);

// MustReadFileCharPtr takes a filename and returns the contents as a
// const char* string. May panic if there are issues.
const char* MustReadFileCharPtr(const char* filename);

// ReadStdin reads from stdin into a String*
const Error* ReadStdin(String* contents);
