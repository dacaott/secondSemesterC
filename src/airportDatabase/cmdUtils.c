#include "cmdUtils.h"

bool isValidArgs(char* argv[], int argc)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return false;
    }
    return true;
}