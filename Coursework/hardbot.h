#define _CRT_SECURE_NO_WARNINGS
#ifndef HARDBOT_H
#define HARDBOT_H
#include "structs.h"



enum action determineNextMove(player_t* bot, int fieldSize);
int calculateScore(player_t* bot, enum action move, int fieldSize);
int generateScore(player_t* bot, int curDepth, int fieldSize);
int calculateMoveScore(player_t* bot, int curDepth, int fieldSize);
int calculateFinalScore(player_t* bot, int fieldSize);

int emptyCellsInRow(player_t* player, int row_n, int fieldSize);
int emptyCellsInCol(player_t* player, int col_n, int fieldSize);
int mergesInRow(player_t* player, int row_n, int fieldSize);
int mergesInCol(player_t* player, int col_n, int fieldSize);
int leftMonotonicityInRow(player_t* player, int row_n, int fieldSize);
int rightMonotonicityInRow(player_t* player, int row_n, int fieldSize);
int leftMonotonicityInCol(player_t* player, int col_n, int fieldSize);
int rightMonotonicityInCol(player_t* player, int col_n, int fieldSize);
int _min(int x1, int x2);
int sumInRow(player_t* player, int row_n, int fieldSize);
int sumInCol(player_t* player, int col_n, int fieldSize);

#endif