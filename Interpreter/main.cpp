#include "types.hpp"
#include "roots.hpp"

s32 main(s32 argc, char** argv) 
{
    if(argc == 1)
    {
        runInteractive();
    }
    else 
    {
        runFiles(argv[1]);
    }
    return 0;
}


