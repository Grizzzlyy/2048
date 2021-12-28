#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "dynmatrix.h"
#include "2048.h"
#include "hardbot.h"

void play2048withBot()
{
    //Создаем структуры параметры игры, игрок-человек, игрок-компьютер и счетчик сделанных ходов
    gamemode_t gameMode;
    player_t human;
    player_t bot;

    printInfoAboutGame();

    selectGameParameters(&gameMode);

    initializePlayerData(&human, gameMode.fieldSize);
    initializePlayerData(&bot, gameMode.fieldSize);

    //Основной алгоритм, сама игра, длится пока кто-то не выиграет или не проиграет
    for (int turnsCount = 0; human.isWin != 1 && bot.isWin != 1 && human.isLose != 1 && bot.isLose != 1; turnsCount++)
    {
        printGame(human, bot, gameMode, turnsCount); //Выводим поле
        doHumanStep(&human, gameMode.fieldSize); //Делаем ход
        if (gameMode.selectedBot) //Бот делает ход
            doBotStepHard(&bot, gameMode.fieldSize);
        else
            doBotStepEasy(&bot, gameMode.fieldSize);
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

    deleteMatrix(human.field, gameMode.fieldSize);
    deleteMatrix(bot.field, gameMode.fieldSize);
    exit(0);
}

void printInfoAboutGame()
{
    printf("\nWelcome to the game 2048. The authors of this version of the game: Alena Poleshchuk, Sofia Kharitonova, Daroslav Skiba.\n");
}

void selectGameParameters(gamemode_t* gameMode)
{
fieldSizeChoice:
    printf("\nSelect the size of the playing field:\n"
        "0) 4x4\n"
        "1) 6x6\n"
        "2) 8x8\n"
        "9) End the program\n\n"
        "Your choice:");
    scanf("%d", &(gameMode->fieldSize));
    switch (gameMode->fieldSize)
    {
    case 0:
        gameMode->fieldSize = 4;
        break;
    case 1:
        gameMode->fieldSize = 6;
        break;
    case 2:
        gameMode->fieldSize = 8;
        break;
    case 9:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto fieldSizeChoice;
    }

finalNumberChoice:
    printf("\nSelect the number at which the game will end:\n"
        "0) 2048\n"
        "1) 4096\n"
        "9) End the program\n\n"
        "Your choice:");
    scanf("%d", &(gameMode->finalNumber));
    switch (gameMode->finalNumber)
    {
    case 0:
        gameMode->finalNumber = 2048;
        break;
    case 1:
        gameMode->finalNumber = 4096;
        break;
    case 9:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto finalNumberChoice;
    }

botChoice:
    printf("\nSelect you bot-opponent:\n"
        "0) Easy-mode bot\n"
        "1) Hard-mode bot\n"
        "9) End the program\n\n"
        "Your choice:");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 0:
    case 1:
        gameMode->selectedBot = choice;
        return;
    case 9:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto botChoice;
    }
}

void initializePlayerData(player_t* player, int fieldSize)
{
    player->isWin = 0;
    player->isLose = 0;
    player->score = 0;
    player->field = (unsigned short**)createMatrix(fieldSize);
    spawnNumberOnField(player->field, fieldSize);
    spawnNumberOnField(player->field, fieldSize);
}

void spawnNumberOnField(unsigned short** field, int fieldSize)
{
    srand(time(NULL));
    if (rand() % 10 == 0)
    {
        //Спавним 4 на поле с вероятностью 0.1
        while (1)
        {
            int x = rand() % fieldSize;
            int y = rand() % fieldSize;
            if (field[x][y] == 0)
            {
                field[x][y] = 4;
                break;
            }
        }
    }
    else
    {
        //Спавним 2 на поле с вероятностью 0.9
        while (1)
        {
            int x = rand() % fieldSize;
            int y = rand() % fieldSize;
            if (field[x][y] == 0)
            {
                field[x][y] = 2;
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
    for (int i = 0; i < gameMode.fieldSize; i++)
    {
        for (int j = 0; j < gameMode.fieldSize; j++)
            printf("----------------");
        printf("\n");
        for (int j = 0; j < gameMode.fieldSize; j++)
            printf("|\t%hu\t", human.field[i][j]);
        printf("|\n");
    }
    for (int j = 0; j < gameMode.fieldSize; j++)
        printf("----------------");
    printf("\n\n");
    printf("Bot's score: %d\n", bot.score);
    for (int i = 0; i < gameMode.fieldSize; i++)
    {
        for (int j = 0; j < gameMode.fieldSize; j++)
            printf("----------------");
        printf("\n");
        for (int j = 0; j < gameMode.fieldSize; j++)
            printf("|\t%hu\t", bot.field[i][j]);
        printf("|\n");
    }
    for (int j = 0; j < gameMode.fieldSize; j++)
        printf("----------------");
    printf("\n");
    printf("\n0 - swipe up\n1 - swipe down\n2 - swipe left\n3 - swipe right\n9 - end program\n\n");
}

void swipeFieldAndChangeScore(player_t* player, int fieldSize, enum action action)
{
    switch (action)
    {
    case UP:
        //1)Сдвигаем все элементы к верхнему краю
    //Перебираем столбцы
        for (int i = 0; i < fieldSize; i++)
        {
            //Перебираем элементы столбцов
            for (int j = 0; j < fieldSize; j++)
            {
                int k = j;
                //Пропускаем нулевые элементы
                while (k < fieldSize && player->field[k][i] == 0)
                {
                    k++;
                }
                //for (k = j; k <= fieldSize-1 && matrix[k][i] == 0; k++);
                if (k == fieldSize)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = player->field[j][i];
                    player->field[j][i] = player->field[k][i];
                    player->field[k][i] = buffer;
                }
            }
        }
        //2)Складываем соседние одинаковые элементы в столбцах и добавляем к счету
        //Перебираем столбцы
        for (int i = 0; i < fieldSize; i++)
        {
            //Перебираем элементы столбцов
            for (int j = 0; j < fieldSize - 1; j++)
            {
                //Если они равны, то склеиваем,увеличиваем счет и подтягиваем остальные плитки
                if (player->field[j][i] == player->field[j + 1][i] && player->field[j][i] != 0)
                {
                    player->field[j][i] *= 2;
                    player->score += player->field[j][i];
                    int k;
                    for (k = j + 1; k < fieldSize - 1; k++)
                    {
                        player->field[k][i] = player->field[k + 1][i];
                    }
                    player->field[k][i] = 0;
                }
            }
        }
        break;
    case DOWN:
        //1)Сдвигаем все элементы к нижнему краю
    //Перебираем столбцы
        for (int i = 0; i < fieldSize; i++)
        {
            //Перебираем элементы столбцов
            for (int j = fieldSize - 1; j >= 0; j--)
            {
                int k = j;
                //Пропускаем нулевые элементы
                while (k >= 0 && player->field[k][i] == 0)
                {
                    k--;
                }
                if (k == -1)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = player->field[j][i];
                    player->field[j][i] = player->field[k][i];
                    player->field[k][i] = buffer;
                }
            }
        }
        //2)Складываем соседние одинаковые элементы в столбцах и добавляем к счету
        //Перебираем столбцы
        for (int i = 0; i < fieldSize; i++)
        {
            //Перебираем элементы столбцов
            for (int j = fieldSize - 1; j >= 1; j--)
            {
                //Если они равны, то склеиваем,увеличиваем счет и подтягиваем остальные плитки
                if (player->field[j][i] == player->field[j - 1][i] && player->field[j][i] != 0)
                {
                    player->field[j][i] *= 2;
                    player->score += player->field[j][i];
                    int k;
                    for (k = j - 1; k >= 1; k--)
                    {
                        player->field[k][i] = player->field[k - 1][i];
                    }
                    player->field[k][i] = 0;
                }
            }
        }
        break;
    case LEFT:
        //1)Сдвигаем все элементы к левому краю
    //Перебираем строки
        for (int i = 0; i < fieldSize; i++)
        {
            //Перебираем элементы строк
            for (int j = 0; j < fieldSize; j++)
            {
                int k = j;
                //Пропускаем нулевые элементы
                while (k < fieldSize && player->field[i][k] == 0)
                {
                    k++;
                }
                if (k == fieldSize)
                    break;
                else
                {
                    unsigned short buffer;
                    buffer = player->field[i][j];
                    player->field[i][j] = player->field[i][k];
                    player->field[i][k] = buffer;
                }
            }
        }
        //2)Складываем соседние одинаковые элементы в столбцах и добавляем к счету
        //Перебираем строки
        for (int i = 0; i < fieldSize; i++)
        {
            //Перебираем элементы строк
            for (int j = 0; j < fieldSize - 1; j++)
            {
                //Если они равны, то склеиваем,увеличиваем счет и подтягиваем остальные плитки
                if (player->field[i][j] == player->field[i][j + 1] && player->field[i][j] != 0)
                {
                    player->field[i][j] *= 2;
                    player->score += player->field[i][j];
                    int k;
                    for (k = j + 1; k < fieldSize - 1; k++)
                    {
                        player->field[i][k] = player->field[i][k + 1];
                    }
                    player->field[i][k] = 0;
                }
            }
        }
        break;
    case RIGHT:
        //1)Сдвигаем все элементы к правому краю
    //Перебираем строки
        for (int i = 0; i < fieldSize; i++)
        {
            //Перебираем элементы строк
            for (int j = fieldSize - 1; j >= 0; j--)
            {
                int k = j;
                //Пропускаем нулевые элементы
                while (k >= 0 && player->field[i][k] == 0)
                {
                    k--;
                }
                if (k == -1)
                    break;
                else if (j != k)
                {
                    unsigned short buffer;
                    buffer = player->field[i][j];
                    player->field[i][j] = player->field[i][k];
                    player->field[i][k] = buffer;
                }
            }
        }
        //2)Складываем соседние одинаковые элементы в столбцах и добавляем к счету
        //Перебираем строки
        for (int i = 0; i < fieldSize; i++)
        {
            //Перебираем элементы строк
            for (int j = fieldSize - 1; j > 0; j--)
            {
                //Если они равны, то склеиваем,увеличиваем счет и подтягиваем остальные плитки
                if (player->field[i][j] == player->field[i][j - 1] && player->field[i][j] != 0)
                {
                    player->field[i][j] *= 2;
                    player->score += player->field[i][j];
                    int k;
                    for (k = j - 1; k > 0; k--)
                    {
                        player->field[i][k] = player->field[i][k - 1];
                    }
                    player->field[i][k] = 0;
                }
            }
        }
        break;
    default:
        break;
    }
}

int doAction(player_t* player, int fieldSize, enum action action)
{
    player_t simulation; //Создаем симуляцию
    initializePlayerData(&simulation, fieldSize);
    copyStruct(*player, &simulation, fieldSize); //Копируем туда данные игрока

    swipeFieldAndChangeScore(&simulation, fieldSize, action); //Проводим действия в симуляции

    if (cmpMatrix(simulation.field, player->field, fieldSize)) //Если происходят изменения, то делаем этот ход и возвращаем 0
    {
        copyStruct(simulation, player, fieldSize);
        deleteMatrix(simulation.field, fieldSize);
        return 0; //Действие сделано
    }
    else //Если изменений нет, то так нельзя ходить
    {
        //printf("You cannot do this action, please change your choice\n");
        deleteMatrix(simulation.field, fieldSize);
        return 1;//Действие не сделано
    }
}

void doHumanStep(player_t* human, int fieldSize)
{
    //Пасхалочка, без getchar scanf считывает не то, что нужно(прими это как данность)
    //Не баг, а фича =)
    getchar();
    int counterOfImpossibleMoves = 0; //Счетчик кол-ва невозможных ходов. Если равен 4 (никуда нельзя сходить), то игрок проиграл.
choice:
    printf("Enter action:");
    int choice;
    scanf("%d", &choice);
    if (choice == 0 || choice == 1 || choice == 2 || choice == 3)
    {
        enum action action = choice;
        if (doAction(human, fieldSize, action))
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
            spawnNumberOnField(human->field, fieldSize);
            return;
        }
    }
    else if (choice == 9)
        exit(0);
    else
    {
        printf("Wrong number, please try again\n");
        goto choice;
    }
}

void doBotStepEasy(player_t* bot, int fieldSize)
{
    //Данный бот выбирает рандомную сторону и свайпает туда
    int action = rand() % 4;
    int counterOfImpossibleMoves = 0;
    do
    {
        if (doAction(bot, fieldSize, action))
        {
            counterOfImpossibleMoves++;
            action = (action + 1) % 4;
        }
        else
        {
            spawnNumberOnField(bot->field, fieldSize);
            return;
        }
    } while (counterOfImpossibleMoves != 4);
    bot->isLose = 1;
}

void doBotStepHard(player_t* bot, int fieldSize)
{
    enum action bestMove = determineNextMove(bot->field, fieldSize);
    if (bestMove == NONE)
    {
        bot->isLose = 1;
        return;
    }
    doAction(bot, fieldSize, bestMove);
    spawnNumberOnField(bot->field, fieldSize);
}

void copyStruct(player_t sourceStruct, player_t* changingStruct, int fieldSize)
{
    changingStruct->score = sourceStruct.score;
    changingStruct->isWin = sourceStruct.isWin;
    changingStruct->isLose = sourceStruct.isLose;
    copyMatrix(sourceStruct.field, changingStruct->field, fieldSize);
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
    for (int i = 0; i < gameMode.fieldSize; i++)
    {
        for (int j = 0; j < gameMode.fieldSize; j++)
        {
            if (player1->field[i][j] == gameMode.finalNumber)
            {
                player1->isWin = 1;
                break;
            }
            else if (player2->field[i][j] == gameMode.finalNumber)
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

    initializePlayerData(&bot1, gameMode.fieldSize);
    initializePlayerData(&bot2, gameMode.fieldSize);

    //Основной алгоритм, сама игра, длится пока кто-то не выиграет или не проиграет
    for (int turnsCount = 0; bot1.isWin != 1 && bot2.isWin != 1 && bot1.isLose != 1 && bot2.isLose != 1; turnsCount++)
    {
        printBotsGame(bot1, bot2, gameMode, turnsCount); //Выводим поле
        doBotStepEasy(&bot1, gameMode.fieldSize);
        doBotStepHard(&bot2, gameMode.fieldSize);
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

    deleteMatrix(bot1.field, gameMode.fieldSize);
    deleteMatrix(bot2.field, gameMode.fieldSize);
    system("pause");
    exit(0);
}

void selectBotsGameParameters(gamemode_t* gameMode)
{
fieldSizeChoice:
    printf("\nSelect the size of the playing field:\n"
        "0) 4x4\n"
        "1) 6x6\n"
        "2) 8x8\n"
        "9) End the program\n\n"
        "Your choice:");
    scanf("%d", &(gameMode->fieldSize));
    switch (gameMode->fieldSize)
    {
    case 0:
        gameMode->fieldSize = 4;
        break;
    case 1:
        gameMode->fieldSize = 6;
        break;
    case 2:
        gameMode->fieldSize = 8;
        break;
    case 9:
        exit(0);
    default:
        printf("Wrong number, try again\n");
        goto fieldSizeChoice;
    }

finalNumberChoice:
    printf("\nSelect the number at which the game will end:\n"
        "0) 2048\n"
        "1) 4096\n"
        "9) End the program\n\n"
        "Your choice:");
    scanf("%d", &(gameMode->finalNumber));
    switch (gameMode->finalNumber)
    {
    case 0:
        gameMode->finalNumber = 2048;
        break;
    case 1:
        gameMode->finalNumber = 4096;
        break;
    case 9:
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
    printf("Bot #1 score: %d\n", bot1.score);
    for (int i = 0; i < gameMode.fieldSize; i++)
    {
        for (int j = 0; j < gameMode.fieldSize; j++)
            printf("----------------");
        printf("\n");
        for (int j = 0; j < gameMode.fieldSize; j++)
            printf("|\t%hu\t", bot1.field[i][j]);
        printf("|\n");
    }
    for (int j = 0; j < gameMode.fieldSize; j++)
        printf("----------------");
    printf("\n\n");
    printf("Bot #2 score: %d\n", bot2.score);
    for (int i = 0; i < gameMode.fieldSize; i++)
    {
        for (int j = 0; j < gameMode.fieldSize; j++)
            printf("----------------");
        printf("\n");
        for (int j = 0; j < gameMode.fieldSize; j++)
            printf("|\t%hu\t", bot2.field[i][j]);
        printf("|\n");
    }
    for (int j = 0; j < gameMode.fieldSize; j++)
        printf("----------------");
    printf("\n");
}