#include "dynmatrix.h"
#include <stdio.h>

//Ñîçäàåò è âîçâðàùàåò äèíàìè÷åñêèé ìàññèâ íóæíîãî ðàçìåðà è íóæíîãî òèïà
//Ïðè ñîçäàíèè íå çàáûâàéòå ïîòîì îñâîáîäèòü ïàìÿòü ôóíêöèåé deleteMatrix!!!
size_t** createMatrix(unsigned short size)
{
    size_t** A = (size_t**)calloc(size, sizeof(size_t*));
    for (int i = 0; i < size; i++)
    {
        A[i] = (size_t*)calloc(size, sizeof(size_t));
    }
    return A;
}

//Îñâîáîæäàåò ïàìÿòü, çàíÿòóþ äèíàìè÷åñêèì ìàññèâîì
void deleteMatrix(unsigned short** matrix, unsigned short size)
{
    for (unsigned short i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

//Êîïèðóåò sourceMatrix â destinationMatrix
void copyMatrix(unsigned short** sMatrix, unsigned short** dMatrix, unsigned short size)
{
    for (unsigned short i = 0; i < size; i++)
    {
        for (unsigned short j = 0; j < size; j += 4)
        {
            dMatrix[i][j] = sMatrix[i][j];
            dMatrix[i][j + 1] = sMatrix[i][j + 1];
            dMatrix[i][j + 2] = sMatrix[i][j + 2];
            dMatrix[i][j + 3] = sMatrix[i][j + 3];
        }
    }
}

//Åñëè ìàòðèöû ðàçíûå, âîçâðàùàåò 1, åñëè îäèíàêîâûå, òî 0
bool cmpMatrix(unsigned short** matrix1, unsigned short** matrix2, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j += 2)
        {
            if (matrix1[i][j] != matrix2[i][j] || matrix1[i][j+1] != matrix2[i][j+1] )
                return 1;
        }
    }
    return 0;
}

void printMatrix(unsigned short** matrix, unsigned short size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j += 4)
        {
            printf("%hu %hu %hu %hu ", matrix[i][j], matrix[i][j + 1], matrix[i][j + 2], matrix[i][j + 3]);
        }
        printf("\n");
    }
}