#include <stdlib.h>
//������� � ���������� ������������ ������ ������� ������� � ������� ����
//��� �������� �� ��������� ����� ���������� ������ �������� deleteDynamicMatrix!!!
size_t** createDynamicMatrix(unsigned short matrixSize)
{
    size_t** A = (size_t**)calloc(matrixSize, sizeof(size_t*));
    for (int i = 0; i < matrixSize; i++)
    {
        A[i] = (size_t*)calloc(matrixSize, sizeof(size_t));
    }
    return A;
}

//����������� ������, ������� ������������ ��������
void deleteDynamicMatrix(unsigned short** matrix, unsigned short matrixSize)
{
    for (unsigned short i = 0; i < matrixSize; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

//�������� sourceMatrix � changingMatrix
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

//���� ������� ������, ���������� 1, ���� ����������, �� 0
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