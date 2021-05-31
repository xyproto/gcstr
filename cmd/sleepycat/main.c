#include <stdlib.h>
#include "gcstr/gcstr.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    String* filename = NewString(argv[1]);
    Print(MustReadFile(filename));
    return EXIT_SUCCESS;
}
