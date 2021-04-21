#ifndef  ARRAYS_MULTIDIMENSIONAL_H
#define  ARRAYS_MULTIDIMENSIONAL_H

#include <stddef.h>

size_t arrays_multidim_get_index(size_t m, size_t n, size_t M);

double arrays_mutlidim_get_element(double* array, size_t m, size_t n, size_t M);

#endif //ARRAYS_MULTIDIMENSIONAL_H