#include "dynmatrix.h"
#include <stdio.h>

//������� � ���������� ������������ ������ ������� ������� � ������� ����
//��� �������� �� ��������� ����� ���������� ������ �������� deleteMatrix!!!
size_t** createMatrix(unsigned short size)
{
    size_t** A = (size_t**)calloc(size, sizeof(size_t*));
    for (int i = 0; i < size; i += 4)
    {
        A[i] = (size_t*)calloc(size, sizeof(size_t));
        A[i + 1] = (size_t*)calloc(size, sizeof(size_t));
        A[i + 2] = (size_t*)calloc(size, sizeof(size_t));
        A[i + 3] = (size_t*)calloc(size, sizeof(size_t));
    }
    return A;
}

//����������� ������, ������� ������������ ��������
void deleteMatrix(unsigned short** matrix, unsigned short size)
{
    for (unsigned short i = 0; i < size; i += 4)
    {
        free(matrix[i]);
        free(matrix[i + 1]);
        free(matrix[i + 2]);
        free(matrix[i + 3]);
    }
    free(matrix);
}

//�������� sourceMatrix � destinationMatrix
void copyMatrix(unsigned short** sMatrix, unsigned short** dMatrix, unsigned short size)
{
    for (unsigned short i = 0; i < size; i++)
    {
        for (unsigned short j = 0; j < size; j++)
        {
            dMatrix[i][j] = sMatrix[i][j];
        }
    }
}

//���� ������� ������, ���������� 1, ���� ����������, �� 0
bool cmpMatrix(unsigned short** matrix1, unsigned short** matrix2, int size)
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

void printMatrix(unsigned short** matrix, unsigned short size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%hu ", matrix[i][j]);
        }
        printf("\n");
    }
}