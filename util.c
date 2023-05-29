#include <stdbool.h>

#include "util.h"

bool comparar_unsigned(const unsigned *a, const unsigned *b)
{
    if (*a == *b) return true;
    return false;
}