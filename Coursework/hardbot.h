#ifndef HARDBOT_H
#define HARDBOT_H

#include "structs.h"

enum action determineNextMove(unsigned short** board, int boardSize);

float calculateScore(unsigned short** board, enum action move, int boardSize);
float generateScore(unsigned short** board, int curDepth, int boardSize);
float calculateMoveScore(unsigned short** board, int curDepth, int boardSize);
float calculateFinalScore(unsigned short** board, int boardSize);

void simulateMove(unsigned short** board, enum action move, int boardSize);
int _min(int x1, int x2);
int emptyCellsInRow(unsigned short** board, int row_n, int boardSize);
int emptyCellsInCol(unsigned short** board, int col_n, int boardSize);
int mergesInRow(unsigned short** board, int row_n, int boardSize);
int mergesInCol(unsigned short** board, int col_n, int boardSize);
int leftMonotonicityInRow(unsigned short** board, int row_n, int boardSize);
int rightMonotonicityInRow(unsigned short** board, int row_n, int boardSize);
int leftMonotonicityInCol(unsigned short** board, int col_n, int boardSize);
int rightMonotonicityInCol(unsigned short** board, int col_n, int boardSize);
int sumInRow(unsigned short** board, int row_n, int boardSize);
int sumInCol(unsigned short** board, int col_n, int boardSize);

#endif