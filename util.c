#include <stdbool.h>

#include "util.h"

bool comparar_unsigned(unsigned *a, unsigned *b)
{
    if (*a == *b) return true;
    return false;
}