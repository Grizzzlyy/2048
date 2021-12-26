#define _CRT_SECURE_NO_WARNINGS
#include "2048.h"
#define MAX_DEPTH 3
#define FIXED_SCORE 0
#define EMPTY_SCORE 0
#define MERGES_SCORE 0
#define MAX_DEPTH 0
#define MONOTONICITY_SCORE 0
#define SUM_SCORE 0

action determineNextMove(player_t* bot, int fieldSize);
int calculateScore(player_t* bot, action move, int fieldSize);
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
int min(int x1, int x2);
int sumInRow(player_t* player, int row_n, int fieldSize);
int sumInCol(player_t* player, int col_n, int fieldSize);

action determineNextMove(player_t *bot, int fieldSize)
{
	action bestMove = NONE;
	int bestScore = 0;
    for (action move = 0; move < 4; move++)
    {
        int score = calculateScore(bot, move, fieldSize);
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}

int calculateScore(player_t* bot, action move, int fieldSize)
{
    player_t tmp;
    copyStruct(*bot, &tmp, fieldSize);
    int possible = doAction(&tmp, fieldSize, move);
    if (possible == 0)
        return 0;
    return generateScore(&tmp, 0, fieldSize);
}

int generateScore(player_t* bot, int curDepth, int fieldSize)
{
    if (curDepth == MAX_DEPTH)
        return calculateFinalScore(bot, curDepth,fieldSize);

    int totalScore = 0;
    for (int  i = 0; i < fieldSize; i++)
    {
        for (int  j = 0; j < fieldSize; j++)
        {
            if (bot->field[i][j] == 0)
            {
                //for 2 spawning
                player_t tmp2;
                copyStruct(*bot, &tmp2, fieldSize);
                tmp2.field[i][j] = 2;
                int moveScore2 = calculateMoveScore(&tmp2, curDepth, fieldSize);
                totalScore += (0.9 * moveScore2);
                //for 4 spawning
                player_t tmp4;
                copyStruct(*bot, &tmp4, fieldSize);
                tmp2.field[i][j] = 4;
                int moveScore4 = calculateMoveScore(&tmp4, curDepth, fieldSize);
                totalScore += (0.1 * moveScore4);
            }
        }
    }
}

int calculateMoveScore(player_t* bot, int curDepth, int fieldSize)
{
    int bestScore = 0;
    for (action move = 0; move < 4; move++)
    {
        player_t tmp;
        copyStruct(*bot, &tmp, fieldSize);
        doAction(&tmp, fieldSize, move);
        int same = 1;
        for (int i = 0; i < fieldSize; i++)
        {
            for (int j = 0; j < fieldSize; j++)
            {
                if (tmp.field[i][j] != bot->field[i][j])
                {
                    same = 0;
                    break;
                }
            }
            if (same == 0)
                break;
        }
        if (same == 0)
        {
            int score = generateScore(&tmp, curDepth + 1, fieldSize);
            if (score > bestScore)
                bestScore = score;
        }
    }
    return bestScore;
}

int calculateFinalScore(player_t* bot, int fieldSize)
{
    int score = 0;
    for (int row_n = 0; row_n < fieldSize; row_n++)
    {
        score += FIXED_SCORE;
        score += EMPTY_SCORE * emptyCellsInRow(bot,row_n, fieldSize);
        score += MERGES_SCORE * mergesInRow(bot, row_n, fieldSize);
        score -= MONOTONICITY_SCORE * min(leftMonotonicityInRow(bot, row_n, fieldSize), rightMonotonicityInRow(bot, row_n, fieldSize));
        score -= SUM_SCORE * sumInRow(bot, row_n, fieldSize);
    }
    for (int col_n = 0; col_n < fieldSize; col_n++)
    {
        score += FIXED_SCORE;
        score += EMPTY_SCORE * emptyCellsInCol(bot, col_n, fieldSize);
        score += MERGES_SCORE * mergesInCol(bot, col_n, fieldSize);
        score -= MONOTONICITY_SCORE * min(leftMonotonicityInCol(bot, col_n, fieldSize),rightMonotonicityInCol(bot, col_n, fieldSize));
        score -= SUM_SCORE * sumInCol(bot, col_n, fieldSize);
    }
    return score;
}

int emptyCellsInRow(player_t *player,int row_n, int fieldSize)
{
    int res = 0;
    for (int i = 0; i < fieldSize; i++)
    {
        if (player->field[row_n][i] == 0)
            res++;
    }
    return res;
}

int emptyCellsInCol(player_t* player, int col_n, int fieldSize)
{
    int res = 0;
    for (int i = 0; i < fieldSize; i++)
    {
        if (player->field[i][col_n] == 0)
            res++;
    }
    return res;
}

int mergesInRow(player_t* player, int row_n, int fieldSize)
{
    int res = 0;
    for (int i = 0; i < fieldSize - 1; i++)
    {
        if (player->field[row_n][i] == player->field[row_n][i+1])
            res++;
    }
    return res;
}

int mergesInCol(player_t* player, int col_n, int fieldSize)
{
    int res = 0;
    for (int i = 0; i < fieldSize - 1; i++)
    {
        if (player->field[i][col_n] == player->field[i+1][col_n])
            res++;
    }
    return res;
}

int leftMonotonicityInRow(player_t* player, int row_n, int fieldSize)
{
    int res = 0;
    int tmp = 0;
    for (int i = 0; i < fieldSize - 1; i++)
    {
        if (player->field[row_n][i] < player->field[row_n][i + 1])
            tmp++;
        else
        {
            res = tmp;
            tmp = 0;
        }
    }
    return res;
}

int rightMonotonicityInRow(player_t* player, int row_n, int fieldSize)
{
    int res = 0;
    int tmp = 0;
    for (int i = 0; i < fieldSize - 1; i++)
    {
        if (player->field[row_n][i] > player->field[row_n][i+1])
            tmp++;
        else
        {
            res = tmp;
            tmp = 0;
        }
    }
    return res;
}

int leftMonotonicityInCol(player_t* player, int col_n, int fieldSize)
{
    int res = 0;
    int tmp = 0;
    for (int i = 0; i < fieldSize - 1; i++)
    {
        if (player->field[i][col_n] < player->field[i + 1][col_n])
            tmp++;
        else
        {
            res = tmp;
            tmp = 0;
        }
    }
    return res;
}

int rightMonotonicityInCol(player_t* player, int col_n, int fieldSize)
{
    int res = 0;
    int tmp = 0;
    for (int i = 0; i < fieldSize - 1; i++)
    {
        if (player->field[i][col_n] > player->field[i + 1][col_n])
            tmp++;
        else
        {
            res = tmp;
            tmp = 0;
        }
    }
    return res;
}

int min(int x1, int x2)
{
    if (x1 < x2)
        return x1;
    else
        return x2;
}

int sumInRow(player_t* player, int row_n, int fieldSize)
{
    int sum = 0;
    for (int i = 0; i < fieldSize; i++)
    {
        sum += player->field[row_n][i];
    }
    return sum;
}

int sumInCol(player_t* player, int col_n, int fieldSize)
{
    int sum = 0;
    for (int i = 0; i < fieldSize; i++)
    {
        sum += player->field[i][col_n];
    }
    return sum;
}