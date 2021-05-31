#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "convenience.h"
#include "error.h"
#include "filedata.h"
#include "gotypes.h"
#include "panic.h"
#include "print.h"
#include "str.h"
#include "stringlist.h"

#define READY NewString("READY")
#define NL NewStringFromChar('\n')

const Error* test_Print()
{
    Print(READY);
    Print(NL);
    return nil;
}

const Error* test_Println()
{
    Println(READY);
    return nil;
}

const Error* test_MustFileData()
{
    String* filename = NewString("testdata/hello.txt");
    FileData* fd = NewFileData(filename);
    const Error* err = FileDataToError(fd);
    if (err != nil) {
        return err;
    }
    Printfu(NewString("%u bytes read.\n"), FileDataLen(fd));
    Printf2(NewString("Contents of %s: %s\n"), filename, FileDataToString(fd));
    return nil;
}

const Error* test_FileData()
{
    String* filename = NewString("testdata/hello.txt");
    FileData* fd = NewFileData(filename);
    if (fd == nil) {
        return NewErrorCharPtr("could not allocate memory for a new FileData struct");
    }
    if (fd->err != nil) {
        return fd->err;
    }
    Printfu(NewString("%u bytes read.\n"), FileDataLen(fd));
    Printf2(NewString("Contents of %s: %s\n"), filename, FileDataToString(fd));
    return nil;
}

const Error* test_Shift()
{
    String* s = NewString("hello");
    Shift(s, 1);
    Printf1(NewString("string shifted 1 to the right: %s\n"), s);
    Shift(s, -1);
    Printf1(NewString("string shifted 1 to the left: %s\n"), s);
    Shift(s, -1);
    Printf1(NewString("string shifted 1 to the left: %s\n"), s);
    return nil;
}

const Error* test_Trim()
{
    String* s1 = NewString("   \na string that is not\ntrimmed  \t   \n  \t\n");
    Print(NewString("|"));
    Print(s1);
    printf("printf: %s\n", StringToCharPtr(s1));
    Println(ListString(s1));
    Println(NewString("|"));

    String* s2 = NewString("   \na string that has been\ntrimmed  \t   \n  \t\n");
    Trim(s2);
    Print(NewString("|"));
    Print(s2);
    Println(NewString("|"));

    String* s3 = NewString("hi   \na string that is not trimmed  \t   \n  \t\n");
    Println(NewString("--- s3 ---"));
    Trim(s3);
    Println(s3);
    printf("%s\n", s3->contents);

    return nil;
}

const Error* test_Find()
{
    FindResult* fr = Find(NewString("hello there"), NewString("there"));
    Println(FindResultToString(fr));
    return nil;
}

const Error* test_Count()
{
    String* s = NewString("a b c d d e f f f f");
    uint c = Count(s, NewString("f"));
    Printfu(NewString("f count: %u\n"), c);

    s = NewString("a b c d d e fu fu fe fe fe fe fe fe fe fu");
    c = Count(s, NewString("fe"));
    Printfu(NewString("fe count: %u\n"), c);
    return nil;
}

const Error* test_MustRead()
{
    Print(MustReadFile(NewString("testdata/hello.txt")));
    printf("%s", MustReadFileCharPtr("testdata/hello.txt"));
    return nil;
}

const Error* test_Slice()
{
    const String* n = NewString("hello there you");
    const String* there = Slice(n, 6, 11);
    Println(there);
    return nil;
}

const Error* test_Combine()
{
    String* n = NewString("hello there you");
    Println(Combine(NewString("STRING: "), n));
    return nil;
}

const Error* test_StringList()
{
    String* n = NewString("hello there you");

    StringList* sl = Fields(n);

    printf("sl length = %u\n", StringListLen(sl));
    PrintStringList(sl);

    Print(NewString("FIRST: "));
    Println(FirstString(sl));

    Print(NewString("LAST: "));
    Println(LastString(sl));

    if (!EqualCharPtr(StringListPop(sl), "you")) {
        return NewErrorCharPtr("the last element in the string should be \"you\"");
    }
    if (!EqualCharPtr(StringListPop(sl), "there")) {
        return NewErrorCharPtr("the second element in the string should be \"there\"");
    }
    if (!EqualCharPtr(StringListPop(sl), "hello")) {
        return NewErrorCharPtr("the first element in the string should be \"hello\"");
    }
    return nil;
}

const Error* test_Fields()
{
    String* s2 = NewString("hello there you");
    Println(s2);
    StringList* sl = Fields(s2);
    PrintStringList(sl);

    String* s3 = NewString("hello\nthere\nyou\n");
    Println(s3);
    sl = Fields(s3);
    PrintStringList(sl);

    return nil;
}

const Error* test_Lines()
{
    String* s2 = NewString("hello\nthere\nyou\n");
    Println(s2);
    StringList* sl = Lines(s2);
    PrintStringList(sl);
    return nil;
}

const Error* test_SplitChar()
{
    String* s3 = NewString("These\nare\non\nseparate\nlines");
    Println(s3);
    StringList* sl = SplitChar(s3, '\n');
    PrintStringList(sl);
    return nil;
}

const Error* test_Join()
{
    String* s3 = NewString("These\nare\non\nseparate\nlines");
    Println(s3);
    StringList* sl = SplitChar(s3, '\n');
    PrintStringList(sl);
    const String* separatedByArrows = JoinCharPtr(sl, "->");
    Println(separatedByArrows);
    return nil;
}

const Error* test_Split()
{
    String* separatedByArrows = NewString("hello->you->there");
    StringList* sl = Split(separatedByArrows, NewString("->"));
    PrintStringList(sl);
    return nil;
}

int main(int argc, char* argv[])
{
    // Test basic printing
    test_Println();
    test_Print();

    // Test reading in a small text file, and error handling
    const Error* err = test_FileData();
    if (err != nil) {
        String* situation = NewString("reading file");
        panicWhen(situation, err);
    }

    // Test reading in a small text file, and error handling
    err = test_MustFileData();
    if (err != nil) {
        String* situation = NewString("reading file");
         panicWhen(situation, err);
    }

    // Test shifting strings left and right
    panicIfError(test_Shift());

    // Test trimming
    panicIfError(test_Trim());

    // Test string searching
    panicIfError(test_Find());

    // Test string counting
    panicIfError(test_Count());

    // Test the MustRead convenience function (may panic)
    panicIfError(test_MustRead());

    // Test slicing a string, from a position, up to a position (like in Python)
    panicIfError(test_Slice());

    // Test combining two strings
    panicIfError(test_Combine());

    // Test basic use of a StringList
    panicIfError(test_StringList());

    // Test the Fields function that can split a string into fields without whitespace
    panicIfError(test_Fields());

    // Test the Lines function that can split a string into lines
    panicIfError(test_Lines());

    // Test the SplitChar function that can split a string by a character
    panicIfError(test_SplitChar());

    // Test the Join function that can join a StringList* into a String*
    panicIfError(test_Join());

    // Test the Split function that can split a string by a separator string
    panicIfError(test_Split());

    return EXIT_SUCCESS;
}
