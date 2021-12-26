#ifndef DYNMATRIX_H
#define DYNMATRIX_H
#include <stdlib.h>
#include <stdbool.h>
size_t** createDynamicMatrix(unsigned short matrixSize);
void deleteDynamicMatrix(unsigned short** matrix, unsigned short matrixSize);
void copyMatrix(unsigned short** sourceMatrix, unsigned short** changingMatrix, unsigned short matrixSize);
bool isMatricesDiffer(unsigned short** matrix1, unsigned short** matrix2, int size);
#endif