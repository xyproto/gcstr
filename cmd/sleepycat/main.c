#include <stdlib.h>
#include "gcstr/gcstr.h"

int main(int argc, char* argv[])
{
    String* filename = NewString((argc > 1) ? argv[1] : "-");
    Print(MustReadFile(filename));
    return EXIT_SUCCESS;
}
