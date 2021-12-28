#ifndef _2048_H
#define _2048_H

#include "structs.h"

//Запустить игру человека с ботом
void play2048withBot();
//Вывести информацию об игре
void printInfoAboutGame();
//Предложить игроку выбор параметров игры (размер поля, финальное число, выбор бота)
void selectGameParameters(gamemode_t* gameMode);
//Инициализация данных структуры player_t
void initializePlayerData(player_t* player, int fieldSize);
//Заспавнить новое число на поле
void spawnTileOnBoard(unsigned short** field, int fieldSize);
//Вывести в консоль игровое поле с данными
void printGame(player_t human, player_t bot, gamemode_t gameMode, int turnsCounter);
//Свайпнуть клетки поля и поменять счет игрока
void swipeFieldAndChangeScore(player_t* player, int fieldSize, enum action action);
//Функция, которая выполняет действие, если оно возможно, и возвращает 0. Если невозможно, то не выполняет и возвращает 1.
int doAction(player_t* player, int fieldSize, enum action action);
//Ход игрока
void doHumanStep(player_t* human, int fieldSize);
//Ход легкого бота
void doBotStepEasy(player_t* bot, int fieldSize);
//Ход сложного бота
void doBotStepHard(player_t* bot, int fieldSize);
//Копировать данные sourceStruct в changingStruct
void copyStruct(player_t sourceStruct, player_t* changingStruct, int fieldSize);
//Возвращает максимальное число из переданных
int returnMaxScore(int score1, int score2, int score3, int score4);
//Делает значение isWin игрока = 1, если кто-то из игроков выиграл
void checkWin(player_t* player1, player_t* player2, gamemode_t gameMode);

//Запустить игру бота с ботом
void watchBotsGame();
//Выбрать параметры для игры между ботами
void selectBotsGameParameters(gamemode_t* gameMode);
//Вывести в консоль данные игры ботов
void printBotsGame(player_t bot1, player_t bot2, gamemode_t gameMode, int turnsCounter);
#endif