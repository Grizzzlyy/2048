#include <stdlib.h>
//—оздает и возвращает динамический массив нужного размера и нужного типа
//ѕри создании не забывайте потом освободить пам€ть функцией deleteDynamicMatrix!!!
size_t** createDynamicMatrix(unsigned short matrixSize)
{
    size_t** A = (size_t**)calloc(matrixSize, sizeof(size_t*));
    for (int i = 0; i < matrixSize; i++)
    {
        A[i] = (size_t*)calloc(matrixSize, sizeof(size_t));
    }
    return A;
}

//ќсвобождает пам€ть, зан€тую динамическим массивом
void deleteDynamicMatrix(unsigned short** matrix, unsigned short matrixSize)
{
    for (unsigned short i = 0; i < matrixSize; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

// опирует sourceMatrix в changingMatrix
void copyMatrix(unsigned short** sourceMatrix, unsigned short** changingMatrix, unsigned short matrixSize)
{
    for (unsigned short i = 0; i < matrixSize; i++)
    {
        for (unsigned short j = 0; j < matrixSize; j++)
        {
            changingMatrix[i][j] = sourceMatrix[i][j];
        }
    }
}

//≈сли матрицы разные, возвращает 1, если одинаковые, то 0
bool isMatricesDiffer(unsigned short** matrix1, unsigned short** matrix2, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (matrix1[i][j] != matrix2[i][j])
                return 1;
        }
    }
    return 0;
}