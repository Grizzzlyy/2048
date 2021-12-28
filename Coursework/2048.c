#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "2048.h"
#include "dynmatrix.h"
#include "hardbot.h"

void play2048withBot()
{
    //Создаем структуры параметры игры, игрок-человек, игрок-компьютер и счетчик сделанных ходов
    gamemode_t gameMode;
    player_t human;
    player_t bot;

    printInfoAboutGame();

    selectGameParameters(&gameMode);

    initializePlayerData(&human, gameMode.boardSize);
    initializePlayerData(&bot, gameMode.boardSize);

    //Основной алгоритм, сама игра, длится пока кто-то не выиграет или не проиграет
    for (int turnsCount = 0; human.isWin != 1 && bot.isWin != 1 && human.isLose != 1 && bot.isLose != 1; turnsCount++)
    {
        printGame(human, bot, gameMode, turnsCount); //Выводим поле
        doHumanStep(&human, gameMode.boardSize); //Делаем ход
        if (gameMode.selectedBot) //Бот делает ход
            doBotStepHard(&bot, gameMode.boardSize);
        else
            doBotStepEasy(&bot, gameMode.boardSize);
        checkWin(&human, &bot, gameMode); //Устанавливаем значения перменным isWin, если кто-то выиграл
    }

    if (human.isWin || bot.isLose) //Выводим результаты игры
    {
        printf("Congretulations! You win!\n");
    }
    if (human.isLose || bot.isWin)
    {
        printf("You lose =(\n");
    }

    deleteMatrix(human.board, gameMode.boardSize);
    deleteMatrix(bot.board, gameMode.boardSize);
    exit(0);
}

void printInfoAboutGame()
{
    printf("\nWelcome to the game 2048. The authors of this version of the game: Alena Poleshchuk, Sofia Kharitonova, Daroslav Skiba.\n");
}

void selectGameParameters(gamemode_t* gameMode)
{
boardSizeChoice:
    printf("\nSelect the size of the board:\n"
        "1) 4x4\n"
        "2) 6x6\n"
        "3) 8x8\n"
        "0) End the program\n\n"
        "Your choice:");
    scanf("%d", &(gameMode->boardSize));
    switch (gameMode->boardSize)
    {
    case 1:
        gameMode->boardSize = 4;
        break;
    case 2:
        gameMode->boardSize = 6;
        break;
    case 3:
        gameMode->boardSize = 8;
        break;
    case 0:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto boardSizeChoice;
    }

finalNumberChoice:
    printf("\nSelect the number at which the game will end:\n"
        "1) 2048\n"
        "2) 4096\n"
        "0) End the program\n\n"
        "Your choice:");
    scanf("%d", &(gameMode->finalNumber));
    switch (gameMode->finalNumber)
    {
    case 1:
        gameMode->finalNumber = 2048;
        break;
    case 2:
        gameMode->finalNumber = 4096;
        break;
    case 0:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto finalNumberChoice;
    }

botChoice:
    printf("\nSelect you bot-opponent:\n"
        "1) Easy-mode bot (randomizer)\n"
        "2) Hard-mode bot (kind of AI)\n"
        "0) End the program\n\n"
        "Your choice:");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
    case 2:
        gameMode->selectedBot = choice - 1;
        return;
    case 0:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto botChoice;
    }
}

void initializePlayerData(player_t* player, int boardSize)
{
    player->isWin = 0;
    player->isLose = 0;
    player->score = 0;
    player->board = (unsigned short**)createMatrix(boardSize);
    spawnTileOnBoard(player->board, boardSize);
    spawnTileOnBoard(player->board, boardSize);
}

void spawnTileOnBoard(unsigned short** board, int boardSize)
{
    srand(time(NULL));
    if (rand() % 10 == 0)
    {
        //Спавним 4 на поле с вероятностью 0.1
        while (1)
        {
            int x = rand() % boardSize;
            int y = rand() % boardSize;
            if (board[x][y] == 0)
            {
                board[x][y] = 4;
                break;
            }
        }
    }
    else
    {
        //Спавним 2 на поле с вероятностью 0.9
        while (1)
        {
            int x = rand() % boardSize;
            int y = rand() % boardSize;
            if (board[x][y] == 0)
            {
                board[x][y] = 2;
                break;
            }
        }
    }
}

void printGame(player_t human, player_t bot, gamemode_t gameMode, int turnsCounter)
{
    system("cls");
    printf("Selected bot: ");
    if (gameMode.selectedBot)
        printf("Hard\n");
    else
        printf("Easy\n");
    printf("Number to win: %d\n", gameMode.finalNumber);
    printf("Number of turns: %d\n\n", turnsCounter);
    printf("Player's score: %d\n", human.score);
    for (int i = 0; i < gameMode.boardSize; i++)
    {
        for (int j = 0; j < gameMode.boardSize; j++)
            printf("----------------");
        printf("\n");
        for (int j = 0; j < gameMode.boardSize; j++)
            printf("|\t%hu\t", human.board[i][j]);
        printf("|\n");
    }
    for (int j = 0; j < gameMode.boardSize; j++)
        printf("----------------");
    printf("\n\n");
    printf("Bot's score: %d\n", bot.score);
    for (int i = 0; i < gameMode.boardSize; i++)
    {
        for (int j = 0; j < gameMode.boardSize; j++)
            printf("----------------");
        printf("\n");
        for (int j = 0; j < gameMode.boardSize; j++)
            printf("|\t%hu\t", bot.board[i][j]);
        printf("|\n");
    }
    for (int j = 0; j < gameMode.boardSize; j++)
        printf("----------------");
    printf("\n");
    printf("\n1 - swipe up\n2 - swipe down\n3 - swipe left\n4 - swipe right\n0 - end program\n\n");
}

void swipeFieldAndChangeScore(player_t* player, int boardSize, enum action action)
{
    switch (action)
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
                while (k < boardSize && player->board[k][i] == 0)
                {
                    k++;
                }
                //for (k = j; k <= boardSize-1 && matrix[k][i] == 0; k++);
                if (k == boardSize)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = player->board[j][i];
                    player->board[j][i] = player->board[k][i];
                    player->board[k][i] = buffer;
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
                //Если они равны, то склеиваем,увеличиваем счет и подтягиваем остальные плитки
                if (player->board[j][i] == player->board[j + 1][i] && player->board[j][i] != 0)
                {
                    player->board[j][i] *= 2;
                    player->score += player->board[j][i];
                    int k;
                    for (k = j + 1; k < boardSize - 1; k++)
                    {
                        player->board[k][i] = player->board[k + 1][i];
                    }
                    player->board[k][i] = 0;
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
                while (k >= 0 && player->board[k][i] == 0)
                {
                    k--;
                }
                if (k == -1)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = player->board[j][i];
                    player->board[j][i] = player->board[k][i];
                    player->board[k][i] = buffer;
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
                if (player->board[j][i] == player->board[j - 1][i] && player->board[j][i] != 0)
                {
                    player->board[j][i] *= 2;
                    player->score += player->board[j][i];
                    int k;
                    for (k = j - 1; k >= 1; k--)
                    {
                        player->board[k][i] = player->board[k - 1][i];
                    }
                    player->board[k][i] = 0;
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
                while (k < boardSize && player->board[i][k] == 0)
                {
                    k++;
                }
                if (k == boardSize)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = player->board[i][j];
                    player->board[i][j] = player->board[i][k];
                    player->board[i][k] = buffer;
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
                if (player->board[i][j] == player->board[i][j + 1] && player->board[i][j] != 0)
                {
                    player->board[i][j] *= 2;
                    player->score += player->board[i][j];
                    int k;
                    for (k = j + 1; k < boardSize - 1; k++)
                    {
                        player->board[i][k] = player->board[i][k + 1];
                    }
                    player->board[i][k] = 0;
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
                while (k >= 0 && player->board[i][k] == 0)
                {
                    k--;
                }
                if (k == -1)
                    break;
                else if (j != k)
                {
                    unsigned short buffer;
                    buffer = player->board[i][j];
                    player->board[i][j] = player->board[i][k];
                    player->board[i][k] = buffer;
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
                if (player->board[i][j] == player->board[i][j - 1] && player->board[i][j] != 0)
                {
                    player->board[i][j] *= 2;
                    player->score += player->board[i][j];
                    int k;
                    for (k = j - 1; k > 0; k--)
                    {
                        player->board[i][k] = player->board[i][k - 1];
                    }
                    player->board[i][k] = 0;
                }
            }
        }
        break;
    default:
        break;
    }
}

int doAction(player_t* player, int boardSize, enum action action)
{
    player_t simulation; //Создаем симуляцию
    initializePlayerData(&simulation, boardSize);
    copyStruct(*player, &simulation, boardSize); //Копируем туда данные игрока

    swipeFieldAndChangeScore(&simulation, boardSize, action); //Проводим действия в симуляции

    if (cmpMatrix(simulation.board, player->board, boardSize)) //Если происходят изменения, то делаем этот ход и возвращаем 0
    {
        copyStruct(simulation, player, boardSize);
        deleteMatrix(simulation.board, boardSize);
        return 0; //Действие сделано
    }
    else //Если изменений нет, то так нельзя ходить
    {
        printf("You cannot do this action, please change your choice\n");
        deleteMatrix(simulation.board, boardSize);
        return 1;//Действие не сделано
    }
}

void doHumanStep(player_t* human, int boardSize)
{
    //Пасхалочка, без getchar scanf считывает не то, что нужно(прими это как данность)
    //Не баг, а фича =)
    getchar();
    int counterOfImpossibleMoves = 0; //Счетчик кол-ва невозможных ходов. Если равен 4 (никуда нельзя сходить), то игрок проиграл.
choice:
    printf("Enter action:");
    int choice;
    scanf("%d", &choice);
    if (choice == 1 || choice == 2 || choice == 3 || choice == 4)
    {
        enum action action = choice - 1;
        if (doAction(human, boardSize, action))
        {
            counterOfImpossibleMoves++;
            if (counterOfImpossibleMoves == 4)
            {
                human->isLose = 1;
            }
            else
            {
                goto choice;
            }
            return;
        }
        else
        {
            spawnTileOnBoard(human->board, boardSize);
            return;
        }
    }
    else if (choice == 0)
        exit(0);
    else
    {
        printf("Wrong number, please try again\n");
        goto choice;
    }
}

void doBotStepEasy(player_t* bot, int boardSize)
{
    //Данный бот выбирает рандомную сторону и свайпает туда
    int action = rand() % 4;
    int counterOfImpossibleMoves = 0;
    do
    {
        if (doAction(bot, boardSize, action))
        {
            counterOfImpossibleMoves++;
            action = (action + 1) % 4;
        }
        else
        {
            spawnTileOnBoard(bot->board, boardSize);
            return;
        }
    } while (counterOfImpossibleMoves != 4);
    bot->isLose = 1;
}

void doBotStepHard(player_t* bot, int boardSize)
{
    enum action bestMove = determineNextMove(bot->board, boardSize);
    if (bestMove == NONE)
    {
        bot->isLose = 1;
        return;
    }
    doAction(bot, boardSize, bestMove);
    spawnTileOnBoard(bot->board, boardSize);
}

void copyStruct(player_t sStruct, player_t* dStruct, int boardSize)
{
    dStruct->score = sStruct.score;
    dStruct->isWin = sStruct.isWin;
    dStruct->isLose = sStruct.isLose;
    copyMatrix(sStruct.board, dStruct->board, boardSize);
}

int returnMaxScore(int score1, int score2, int score3, int score4)
{
    int maxScore = score1;
    if (score2 > maxScore)
        maxScore = score2;
    if (score3 > maxScore)
        maxScore = score3;
    if (score4 > maxScore)
        maxScore = score4;
    return maxScore;
}

void checkWin(player_t* player1, player_t* player2, gamemode_t gameMode)
{
    for (int i = 0; i < gameMode.boardSize; i++)
    {
        for (int j = 0; j < gameMode.boardSize; j++)
        {
            if (player1->board[i][j] == gameMode.finalNumber)
            {
                player1->isWin = 1;
                break;
            }
            else if (player2->board[i][j] == gameMode.finalNumber)
            {
                player2->isWin = 1;
                break;
            }
        }
    }
}


void watchBotsGame()
{
    //Создаем структуры параметры игры, бот1, бот2 и счетчик сделанных ходов
    gamemode_t gameMode;
    player_t bot1;
    player_t bot2;

    printInfoAboutGame();

    selectBotsGameParameters(&gameMode);

    initializePlayerData(&bot1, gameMode.boardSize);
    initializePlayerData(&bot2, gameMode.boardSize);

    //Основной алгоритм, сама игра, длится пока кто-то не выиграет или не проиграет
    for (int turnsCount = 0; bot1.isWin != 1 && bot2.isWin != 1 && bot1.isLose != 1 && bot2.isLose != 1; turnsCount++)
    {
        printBotsGame(bot1, bot2, gameMode, turnsCount); //Выводим поле
        doBotStepEasy(&bot1, gameMode.boardSize);
        doBotStepHard(&bot2, gameMode.boardSize);
        checkWin(&bot1, &bot2, gameMode); //Устанавливаем значения перменным isWin, если кто-то выиграл
    }

    if (bot1.isWin || bot2.isLose) //Выводим результаты игры
    {
        printf("Bot #1 win!\n");
    }
    if (bot1.isLose || bot2.isWin)
    {
        printf("Bot #2 win!\n");
    }

    deleteMatrix(bot1.board, gameMode.boardSize);
    deleteMatrix(bot2.board, gameMode.boardSize);
    system("pause");
    exit(0);
}

void selectBotsGameParameters(gamemode_t* gameMode)
{
boardSizeChoice:
    printf("\nSelect the size of the board:\n"
        "1) 4x4\n"
        "2) 6x6\n"
        "3) 8x8\n"
        "0) End the program\n\n"
        "Your choice:");
    scanf("%d", &(gameMode->boardSize));
    switch (gameMode->boardSize)
    {
    case 1:
        gameMode->boardSize = 4;
        break;
    case 2:
        gameMode->boardSize = 6;
        break;
    case 3:
        gameMode->boardSize = 8;
        break;
    case 0:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto boardSizeChoice;
    }

finalNumberChoice:
    printf("\nSelect the number at which the game will end:\n"
        "1) 2048\n"
        "2) 4096\n"
        "0) End the program\n\n"
        "Your choice:");
    scanf("%d", &(gameMode->finalNumber));
    switch (gameMode->finalNumber)
    {
    case 1:
        gameMode->finalNumber = 2048;
        break;
    case 2:
        gameMode->finalNumber = 4096;
        break;
    case 0:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto finalNumberChoice;
    }

    gameMode->selectedBot = 1;
}

void printBotsGame(player_t bot1, player_t bot2, gamemode_t gameMode, int turnsCounter)
{
    system("cls");
    printf("Number to win: %d\n", gameMode.finalNumber);
    printf("Number of turns: %d\n\n", turnsCounter);
    printf("Bot #1 (Easy) score: %d\n", bot1.score);
    for (int i = 0; i < gameMode.boardSize; i++)
    {
        for (int j = 0; j < gameMode.boardSize; j++)
            printf("----------------");
        printf("\n");
        for (int j = 0; j < gameMode.boardSize; j++)
            printf("|\t%hu\t", bot1.board[i][j]);
        printf("|\n");
    }
    for (int j = 0; j < gameMode.boardSize; j++)
        printf("----------------");
    printf("\n\n");
    printf("Bot #2 (Hard) score: %d\n", bot2.score);
    for (int i = 0; i < gameMode.boardSize; i++)
    {
        for (int j = 0; j < gameMode.boardSize; j++)
            printf("----------------");
        printf("\n");
        for (int j = 0; j < gameMode.boardSize; j++)
            printf("|\t%hu\t", bot2.board[i][j]);
        printf("|\n");
    }
    for (int j = 0; j < gameMode.boardSize; j++)
        printf("----------------");
    printf("\n");
}