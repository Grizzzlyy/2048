#ifndef DYNMATRIX_H
#define DYNMATRIX_H

#include <stdlib.h>
#include <stdbool.h>

size_t** createMatrix(unsigned short size);
void deleteMatrix(unsigned short** matrix, unsigned short size);
void copyMatrix(unsigned short** sMatrix, unsigned short** dMatrix, unsigned short size);
bool cmpMatrix(unsigned short** matrix1, unsigned short** matrix2, int size);

#endif