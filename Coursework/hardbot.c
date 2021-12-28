#include "hardbot.h"
#include "dynmatrix.h"

#define MAX_DEPTH 2
#define FIXED_WEIGHT 0
#define EMPTY_WEIGHT 5
#define MERGES_WEIGHT 2
#define MONOTONICITY_WEIGHT 3
#define SUM_WEIGHT 1

//determines best next move (the biggest probability to win)
enum action determineNextMove(unsigned short** board, int boardSize)
{
    enum action bestMove = NONE;
    float bestScore = 0;
    for (enum action move = 0; move < 4; move++)
    {
        float score = calculateScore(board, move, boardSize);
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}
//calculates the score of the move (how good is this move)
float calculateScore(unsigned short** board, enum action move, int boardSize)
{
    unsigned short** newBoard;
    newBoard = (unsigned short**)createMatrix(boardSize);
    copyMatrix(board, newBoard, boardSize);
    simulateMove(newBoard, move, boardSize);

    if (cmpMatrix(newBoard, board, boardSize) == 0)
    {
        deleteMatrix(newBoard, boardSize);
        return 0;
    }
        
    float score = generateScore(newBoard, 0, boardSize);
    deleteMatrix(newBoard, boardSize);
    return score;
}
//generates score of this move (how good is this move), recursive function
float generateScore(unsigned short** board, int curDepth, int boardSize)
{
    if (curDepth == MAX_DEPTH)
        return calculateFinalScore(board, boardSize);

    float totalScore = 0;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (board[i][j] == 0)
            {
                //for 2 spawning
                unsigned short** newBoard2;
                newBoard2 = (unsigned short**)createMatrix(boardSize);
                copyMatrix(board, newBoard2, boardSize);
                newBoard2[i][j] = 2;
                float moveScore2 = calculateMoveScore(newBoard2, curDepth, boardSize);
                deleteMatrix(newBoard2, boardSize);
                totalScore += (0.9 * moveScore2);
                //for 4 spawning
                unsigned short** newBoard4;
                newBoard4 = (unsigned short**)createMatrix(boardSize);
                copyMatrix(board, newBoard4, boardSize);
                newBoard4[i][j] = 4;
                float moveScore4 = calculateMoveScore(newBoard4, curDepth, boardSize);
                deleteMatrix(newBoard4, boardSize);
                totalScore += (0.1 * moveScore4);
            }
        }
    }
    return totalScore;
}
//recursive function
float calculateMoveScore(unsigned short** board, int curDepth, int boardSize)
{
    float bestScore = 0;
    for (enum action move = 0; move < 4; move++)
    {
        unsigned short** newBoard;
        newBoard = (unsigned short**)createMatrix(boardSize);
        copyMatrix(board, newBoard, boardSize);
        simulateMove(newBoard, move, boardSize);
        if (cmpMatrix(board, newBoard, boardSize) == 1)
        {
            float score = generateScore(newBoard, curDepth + 1, boardSize);
            bestScore = (score > bestScore) ? score : bestScore;
        }
        deleteMatrix(newBoard, boardSize);
    }
    return bestScore;
}
//calculates final score of this move (after that it goes to calculateScore and determineNextMove)
float calculateFinalScore(unsigned short** board, int boardSize)
{
    float score = 0;
    for (int row_n = 0; row_n < boardSize; row_n++)
    {
        score += FIXED_WEIGHT;
        score += EMPTY_WEIGHT * emptyCellsInRow(board, row_n, boardSize);
        score += MERGES_WEIGHT * mergesInRow(board, row_n, boardSize);
        score -= MONOTONICITY_WEIGHT * _min(leftMonotonicityInRow(board, row_n, boardSize), rightMonotonicityInRow(board, row_n, boardSize));
        score -= SUM_WEIGHT * sumInRow(board, row_n, boardSize);
    }
    for (int col_n = 0; col_n < boardSize; col_n++)
    {
        score += FIXED_WEIGHT;
        score += EMPTY_WEIGHT * emptyCellsInCol(board, col_n, boardSize);
        score += MERGES_WEIGHT * mergesInCol(board, col_n, boardSize);
        score -= MONOTONICITY_WEIGHT * _min(leftMonotonicityInCol(board, col_n, boardSize), rightMonotonicityInCol(board, col_n, boardSize));
        score -= SUM_WEIGHT * sumInCol(board, col_n, boardSize);
    }
    return score;
}
//swipe board in some direction
void simulateMove(unsigned short** board, enum action move, int boardSize)
{

    switch (move)
    {
    case UP:
        //1)Сдвигаем все элементы к верхнему краю
        //Перебираем столбцы
        for (int i = 0; i < boardSize; i++)
        {
            //Перебираем элементы столбцов
            for (int j = 0; j < boardSize; j++)
            {
                int k = j;
                //Пропускаем нулевые элементы
                while (k < boardSize && board[k][i] == 0)
                {
                    k++;
                }
                //for (k = j; k <= fieldSize-1 && matrix[k][i] == 0; k++);
                if (k == boardSize)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = board[j][i];
                    board[j][i] = board[k][i];
                    board[k][i] = buffer;
                }
            }
        }
        //2)Складываем соседние одинаковые элементы в столбцах и добавляем к счету
        //Перебираем столбцы
        for (int i = 0; i < boardSize; i++)
        {
            //Перебираем элементы столбцов
            for (int j = 0; j < boardSize - 1; j++)
            {
                //Если они равны, то склеиваем и подтягиваем остальные плитки
                if (board[j][i] == board[j + 1][i] && board[j][i] != 0)
                {
                    board[j][i] *= 2;
                    int k;
                    for (k = j + 1; k < boardSize - 1; k++)
                    {
                        board[k][i] = board[k + 1][i];
                    }
                    board[k][i] = 0;
                }
            }
        }
        break;
    case DOWN:
        //1)Сдвигаем все элементы к нижнему краю
    //Перебираем столбцы
        for (int i = 0; i < boardSize; i++)
        {
            //Перебираем элементы столбцов
            for (int j = boardSize - 1; j >= 0; j--)
            {
                int k = j;
                //Пропускаем нулевые элементы
                while (k >= 0 && board[k][i] == 0)
                {
                    k--;
                }
                if (k == -1)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = board[j][i];
                    board[j][i] = board[k][i];
                    board[k][i] = buffer;
                }
            }
        }
        //2)Складываем соседние одинаковые элементы в столбцах и добавляем к счету
        //Перебираем столбцы
        for (int i = 0; i < boardSize; i++)
        {
            //Перебираем элементы столбцов
            for (int j = boardSize - 1; j >= 1; j--)
            {
                //Если они равны, то склеиваем,увеличиваем счет и подтягиваем остальные плитки
                if (board[j][i] == board[j - 1][i] && board[j][i] != 0)
                {
                    board[j][i] *= 2;
                    int k;
                    for (k = j - 1; k >= 1; k--)
                    {
                        board[k][i] = board[k - 1][i];
                    }
                    board[k][i] = 0;
                }
            }
        }
        break;
    case LEFT:
        //1)Сдвигаем все элементы к левому краю
    //Перебираем строки
        for (int i = 0; i < boardSize; i++)
        {
            //Перебираем элементы строк
            for (int j = 0; j < boardSize; j++)
            {
                int k = j;
                //Пропускаем нулевые элементы
                while (k < boardSize && board[i][k] == 0)
                {
                    k++;
                }
                if (k == boardSize)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = board[i][j];
                    board[i][j] = board[i][k];
                    board[i][k] = buffer;
                }
            }
        }
        //2)Складываем соседние одинаковые элементы в столбцах и добавляем к счету
        //Перебираем строки
        for (int i = 0; i < boardSize; i++)
        {
            //Перебираем элементы строк
            for (int j = 0; j < boardSize - 1; j++)
            {
                //Если они равны, то склеиваем,увеличиваем счет и подтягиваем остальные плитки
                if (board[i][j] == board[i][j + 1] && board[i][j] != 0)
                {
                    board[i][j] *= 2;
                    //player->score += board[i][j];
                    int k;
                    for (k = j + 1; k < boardSize - 1; k++)
                    {
                        board[i][k] = board[i][k + 1];
                    }
                    board[i][k] = 0;
                }
            }
        }
        break;
    case RIGHT:
        //1)Сдвигаем все элементы к правому краю
    //Перебираем строки
        for (int i = 0; i < boardSize; i++)
        {
            //Перебираем элементы строк
            for (int j = boardSize - 1; j >= 0; j--)
            {
                int k = j;
                //Пропускаем нулевые элементы
                while (k >= 0 && board[i][k] == 0)
                {
                    k--;
                }
                if (k == -1)
                    break;
                else if (j != k)
                {
                    unsigned short buffer;
                    buffer = board[i][j];
                    board[i][j] = board[i][k];
                    board[i][k] = buffer;
                }
            }
        }
        //2)Складываем соседние одинаковые элементы в столбцах и добавляем к счету
        //Перебираем строки
        for (int i = 0; i < boardSize; i++)
        {
            //Перебираем элементы строк
            for (int j = boardSize - 1; j > 0; j--)
            {
                //Если они равны, то склеиваем,увеличиваем счет и подтягиваем остальные плитки
                if (board[i][j] == board[i][j - 1] && board[i][j] != 0)
                {
                    board[i][j] *= 2;
                    //player->score += board[i][j];
                    int k;
                    for (k = j - 1; k > 0; k--)
                    {
                        board[i][k] = board[i][k - 1];
                    }
                    board[i][k] = 0;
                }
            }
        }
        break;
    default:
        break;
    }
    //swipeFieldAndChangeScore(bot, fieldSize, move);
}

int _min(int x1, int x2)
{
    return (x1 < x2) ? x1 : x2;
}

int emptyCellsInRow(unsigned short** board, int row_n, int boardSize)
{
    int res = 0;
    for (int i = 0; i < boardSize; i++)
    {
        if (board[row_n][i] == 0)
            res++;
    }
    return res;
}

int emptyCellsInCol(unsigned short** board, int col_n, int boardSize)
{
    int res = 0;
    for (int i = 0; i < boardSize; i++)
    {
        if (board[i][col_n] == 0)
            res++;
    }
    return res;
}

int mergesInRow(unsigned short** board, int row_n, int boardSize)
{
    int res = 0;
    for (int i = 0; i < boardSize - 1; i++)
    {
        if (board[row_n][i] == board[row_n][i + 1])
            res++;
    }
    return res;
}

int mergesInCol(unsigned short** board, int col_n, int boardSize)
{
    int res = 0;
    for (int i = 0; i < boardSize - 1; i++)
    {
        if (board[i][col_n] == board[i + 1][col_n])
            res++;
    }
    return res;
}

int leftMonotonicityInRow(unsigned short** board, int row_n, int boardSize)
{
    int res = 0;
    int tmp = 0;
    for (int i = 0; i < boardSize - 1; i++)
    {
        if (board[row_n][i] < board[row_n][i + 1])
            tmp++;
        else
        {
            res = tmp;
            tmp = 0;
        }
    }
    return res;
}

int rightMonotonicityInRow(unsigned short** board, int row_n, int boardSize)
{
    int res = 0;
    int tmp = 0;
    for (int i = 0; i < boardSize - 1; i++)
    {
        if (board[row_n][i] > board[row_n][i + 1])
            tmp++;
        else
        {
            res = tmp;
            tmp = 0;
        }
    }
    return res;
}

int leftMonotonicityInCol(unsigned short** board, int col_n, int boardSize)
{
    int res = 0;
    int tmp = 0;
    for (int i = 0; i < boardSize - 1; i++)
    {
        if (board[i][col_n] < board[i + 1][col_n])
            tmp++;
        else
        {
            res = tmp;
            tmp = 0;
        }
    }
    return res;
}

int rightMonotonicityInCol(unsigned short** board, int col_n, int boardSize)
{
    int res = 0;
    int tmp = 0;
    for (int i = 0; i < boardSize - 1; i++)
    {
        if (board[i][col_n] > board[i + 1][col_n])
            tmp++;
        else
        {
            res = tmp;
            tmp = 0;
        }
    }
    return res;
}

int sumInRow(unsigned short** board, int row_n, int boardSize)
{
    int sum = 0;
    for (int i = 0; i < boardSize; i++)
    {
        sum += board[row_n][i];
    }
    return sum;
}

int sumInCol(unsigned short** board, int col_n, int boardSize)
{
    int sum = 0;
    for (int i = 0; i < boardSize; i++)
    {
        sum += board[i][col_n];
    }
    return sum;
}