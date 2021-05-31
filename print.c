#include <stdio.h>

#include "print.h"
#include "str.h"
#include "stringlist.h"
#include "structs.h"

void Print(const String* s) { fputs(s->contents, stdout); }

void Println(const String* s) { puts(s->contents); }

void Printf1(const String* fmt, const String* a) { Print(Sprintf(fmt, a)); }

void Printf2(const String* fmt, const String* a, const String* b) { Print(Sprintf2(fmt, a, b)); }

void Printfu(const String* fmt, uint u) { Print(SprintfUint(fmt, u)); }

void Flush() { fflush(stdout); }

void PrintStringElement(uint i, const String* s) { printf("\t[%u] %s\n", i, s->contents); }

void PrintStringList(StringList* sl)
{
    puts("StringList*");
    if (sl == nil) {
        puts("\t<empty>");
        return;
    }
    StringListForEach(sl, PrintStringElement);
}
