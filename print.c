#include <stdio.h>

#include "print.h"
#include "str.h"
#include "stringlist.h"
#include "structs.h"

void Print(String* s) { fputs(s->contents, stdout); }

void Println(String* s) { puts(s->contents); }

void Printf1(String* fmt, String* a) { Print(Sprintf1(fmt, a)); }

void Printf2(String* fmt, String* a, String* b) { Print(Sprintf2(fmt, a, b)); }

void Printfu(String* fmt, uint u) { Print(Sprintfu(fmt, u)); }

void Flush() { fflush(stdout); }

void PrintStringElement(uint i, String* s) { printf("\t[%u] %s\n", i, s->contents); }

void PrintStringList(StringList* sl)
{
    puts("StringList*");
    if (sl == nil) {
        puts("\t<empty>");
        return;
    }
    StringListForEach(sl, PrintStringElement);
}
