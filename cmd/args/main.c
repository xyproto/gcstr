#include <stdlib.h>
#include "gcstr/gcstr.h"

int main(int argc, char* argv[])
{
    StringList* sl = StringListFromArgs(argc, argv);
    PrintStringList(sl);
    return EXIT_SUCCESS;
}
