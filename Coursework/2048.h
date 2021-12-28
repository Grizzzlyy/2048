#ifndef _2048_H
#define _2048_H

#include "structs.h"

//��������� ���� �������� � �����
void play2048withBot();
//������� ���������� �� ����
void printInfoAboutGame();
//���������� ������ ����� ���������� ���� (������ ����, ��������� �����, ����� ����)
void selectGameParameters(gamemode_t* gameMode);
//������������� ������ ��������� player_t
void initializePlayerData(player_t* player, int fieldSize);
//���������� ����� ����� �� ����
void spawnTileOnBoard(unsigned short** field, int fieldSize);
//������� � ������� ������� ���� � �������
void printGame(player_t human, player_t bot, gamemode_t gameMode, int turnsCounter);
//��������� ������ ���� � �������� ���� ������
void swipeFieldAndChangeScore(player_t* player, int fieldSize, enum action action);
//�������, ������� ��������� ��������, ���� ��� ��������, � ���������� 0. ���� ����������, �� �� ��������� � ���������� 1.
int doAction(player_t* player, int fieldSize, enum action action);
//��� ������
void doHumanStep(player_t* human, int fieldSize);
//��� ������� ����
void doBotStepEasy(player_t* bot, int fieldSize);
//��� �������� ����
void doBotStepHard(player_t* bot, int fieldSize);
//���������� ������ sourceStruct � changingStruct
void copyStruct(player_t sourceStruct, player_t* changingStruct, int fieldSize);
//���������� ������������ ����� �� ����������
int returnMaxScore(int score1, int score2, int score3, int score4);
//������ �������� isWin ������ = 1, ���� ���-�� �� ������� �������
void checkWin(player_t* player1, player_t* player2, gamemode_t gameMode);

//��������� ���� ���� � �����
void watchBotsGame();
//������� ��������� ��� ���� ����� ������
void selectBotsGameParameters(gamemode_t* gameMode);
//������� � ������� ������ ���� �����
void printBotsGame(player_t bot1, player_t bot2, gamemode_t gameMode, int turnsCounter);
#endif