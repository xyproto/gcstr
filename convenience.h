#pragma once

#include "structs.h"

// MustRead takes a filename and returns the contents as a String*.
// May panic if there are issues.
String* MustRead(String* filename);

// MustReadConstChar takes a filename and returns the contents as a
// const char* string.
// May panic if there are issues.
const char* MustReadConstChar(const char* filename);
