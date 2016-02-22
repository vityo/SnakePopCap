#pragma once
#include <string>
#include "SexyAppFramework/Color.h"

using namespace std;
using namespace Sexy;

namespace Game {
	// ��������������� �������
	const string fontId = "FONT_DEFAULT"; // �����
	const Color fontColor = Color(255, 255, 255); // ���� ������
	const string titleStart = "Press any arrow keys to start!"; // ��������� � ������ ����
	const string titleEnd = "GAME OVER. Press any key to restart!"; // ��������� � ����� ����
	const string titlePriseScore = "SCORE x3!"; // ����� ����� �����
	const string titlePriseSlice = "SLICE!"; // ����� ����� �������
	const string titlePriseSpeed = "SPEED!"; // ����� ����� ��������
	const Color priseScoreColor = Color(255, 255, 0); // ���� ����� �����
	const Color priseSliceColor = Color(0, 255, 0); // ���� ����� �������
	const Color priseSpeedColor = Color(0, 255, 255); // ���� ����� ��������
	const Color backgroundColor = Color(32, 32, 32); // ���� ����
	const Color wallColor = Color(200, 0, 0); // ���� ����
	const Color foodColor = Color(0, 0, 200); // ���� ��� ��� ������
	const Color snakeHeadColor = Color(0, 200, 0); // ���� ������ ������
	const Color snakeColor = Color(0, 150, 0); // ���� ������
	const Color snakeHeadEndColor = Color(0, 100, 0); // ���� ������ ������, ���� ���������
	const Color endCausedWallColor = Color(100, 0, 0); // ���� �����, � ������� ���������
	const float stepPeriod = 0.3f; // ��� ������ ���������� �� ��� ����� � ��������
	const float dtStepCoefficientDefault = 1.0f; // ����������� ������� �� ���������
	const float priseSpeedMul = 2.0f; // ����������� ����� �������
	const float gameOverPeriod = 4.0f; // ������ �������� ������ ����� ���������� ����, ������ �������� �������
	const float priseDisappearPeriod = 7.0f; // ������ ������������ �����
	const float priseActivePeriod = 10.0f; // ������ ���������� �����

	const pair<uint8_t, uint8_t> wallsSize(1, 1); // ������� �������� ����
	const pair<uint8_t, uint8_t> gameInnerSize(16, 13); // ���������� ������� �������� ���� (��� ����)
	const uint8_t snakeFirstSize(3); // ������, � ������� ��������� ������
	const Color endCausedSnakeColor = Color(0, 50, 0); // ���� ���� ������, ���� � ���� ���������
	const long priseScoreMul = 3; // ����������� ����� �����
	const int priseSliceCount = 1; // �������� ������ �� ������� ����� ����� ������ �����
	const int priseSlicePerFood = 2; // �������� ������ ������ ��� ����� ����� ��� ��� ���� �� ����� ����� �������
	const int snakeMinSizeAfterSlice = 1; // ����������� ������ ������ ����� �������
	const long scoreByFood = 10; // ����� �� ���
	const uint8_t foodCountBeforePrise = 2; // ������� ������ ������ ���, ������ ��� �������� ����
	const long scoreCoefficientDefault = 1; // ����������� ���������� ����� �� ���������
}