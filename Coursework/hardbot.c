#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include "hardbot.h"
#include "2048.h"
#include "dynmatrix.h"

#define MAX_DEPTH 3
#define FIXED_WEIGHT 0
#define EMPTY_WEIGHT 5
#define MERGES_WEIGHT 2
#define MONOTONICITY_WEIGHT 3
#define SUM_WEIGHT 1

void simulateMove(player_t* bot, int fieldSize, enum action move)
{
    swipeFieldAndChangeScore(bot, fieldSize, move);
}

enum action determineNextMove(player_t* bot, int fieldSize)
{
    enum action bestMove = NONE;
    int bestScore = 0;
    for (enum action move = 0; move < 4; move++)
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

int calculateScore(player_t* bot, enum action move, int fieldSize)
{
    player_t tmp;
    initializePlayerData(&tmp, fieldSize);
    copyStruct(*bot, &tmp, fieldSize);
    simulateMove(&tmp, fieldSize, move);
    if (isMatricesDiffer(tmp.field, bot->field, fieldSize) == 0)
    {
        deleteDynamicMatrix(tmp.field, fieldSize);
        return 0;
    }
        
    copyStruct(*bot, &tmp, fieldSize);
    /*
    int possible = doAction(&tmp, fieldSize, move);
    copyStruct(*bot, &tmp, fieldSize);
    if (possible == 1)
        return 0;
        */
    int res = generateScore(&tmp, 0, fieldSize);
    deleteDynamicMatrix(tmp.field, fieldSize);
    return res;
}

int generateScore(player_t* bot, int curDepth, int fieldSize)
{
    if (curDepth == MAX_DEPTH)
        return calculateFinalScore(bot, fieldSize);

    int totalScore = 0;
    for (int i = 0; i < fieldSize; i++)
    {
        for (int j = 0; j < fieldSize; j++)
        {
            if (bot->field[i][j] == 0)
            {
                //for 2 spawning
                player_t tmp2;
                initializePlayerData(&tmp2, fieldSize);
                copyStruct(*bot, &tmp2, fieldSize);
                tmp2.field[i][j] = 2;
                int moveScore2 = calculateMoveScore(&tmp2, curDepth, fieldSize);
                deleteDynamicMatrix(tmp2.field, fieldSize);
                totalScore += (0.9 * moveScore2);
                //for 4 spawning
                player_t tmp4;
                initializePlayerData(&tmp4, fieldSize);
                copyStruct(*bot, &tmp4, fieldSize);
                tmp4.field[i][j] = 4;
                int moveScore4 = calculateMoveScore(&tmp4, curDepth, fieldSize);
                deleteDynamicMatrix(tmp4.field, fieldSize);
                totalScore += (0.1 * moveScore4);
            }
        }
    }
}

int calculateMoveScore(player_t* bot, int curDepth, int fieldSize)
{
    int bestScore = 0;
    for (enum action move = 0; move < 4; move++)
    {
        player_t tmp;
        initializePlayerData(&tmp, fieldSize);
        copyStruct(*bot, &tmp, fieldSize);
        simulateMove(&tmp, fieldSize, move);
        /*
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
        */
        if (isMatricesDiffer(tmp.field, bot->field, fieldSize) == 1)
        {
            int score = generateScore(&tmp, curDepth + 1, fieldSize);
            if (score > bestScore)
                bestScore = score;
        }
        deleteDynamicMatrix(tmp.field, fieldSize);
    }
    return bestScore;
}

int calculateFinalScore(player_t* bot, int fieldSize)
{
    int score = 0;
    for (int row_n = 0; row_n < fieldSize; row_n++)
    {
        score += FIXED_WEIGHT;
        score += EMPTY_WEIGHT * emptyCellsInRow(bot, row_n, fieldSize);
        score += MERGES_WEIGHT * mergesInRow(bot, row_n, fieldSize);
        score -= MONOTONICITY_WEIGHT * _min(leftMonotonicityInRow(bot, row_n, fieldSize), rightMonotonicityInRow(bot, row_n, fieldSize));
        score -= SUM_WEIGHT * sumInRow(bot, row_n, fieldSize);
    }
    for (int col_n = 0; col_n < fieldSize; col_n++)
    {
        score += FIXED_WEIGHT;
        score += EMPTY_WEIGHT * emptyCellsInCol(bot, col_n, fieldSize);
        score += MERGES_WEIGHT * mergesInCol(bot, col_n, fieldSize);
        score -= MONOTONICITY_WEIGHT * _min(leftMonotonicityInCol(bot, col_n, fieldSize), rightMonotonicityInCol(bot, col_n, fieldSize));
        score -= SUM_WEIGHT * sumInCol(bot, col_n, fieldSize);
    }
    return score;
}

int emptyCellsInRow(player_t* player, int row_n, int fieldSize)
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
        if (player->field[row_n][i] == player->field[row_n][i + 1])
            res++;
    }
    return res;
}

int mergesInCol(player_t* player, int col_n, int fieldSize)
{
    int res = 0;
    for (int i = 0; i < fieldSize - 1; i++)
    {
        if (player->field[i][col_n] == player->field[i + 1][col_n])
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
        if (player->field[row_n][i] > player->field[row_n][i + 1])
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

int _min(int x1, int x2)
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