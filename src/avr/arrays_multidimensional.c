#include "arrays_multidimensional.h"

size_t arrays_multidim_get_index(size_t height, size_t column, size_t width)
{
    return (height * width + column);
}

void arrays_multidim_get_element(void* array, size_t height, size_t column, size_t width)
{
    return array[arrays_multidim_get_index(height, column, width)];
}