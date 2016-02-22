#pragma once
#include <string>
#include "SexyAppFramework/Color.h"

using namespace std;
using namespace Sexy;

namespace Game {
	// вспомогательные функции
	const string fontId = "FONT_DEFAULT"; // шрифт
	const Color fontColor = Color(255, 255, 255); // цвет текста
	const string titleStart = "Press any arrow keys to start!"; // заголовок в начале игры
	const string titleEnd = "GAME OVER. Press any key to restart!"; // заголовок в конце игры
	const string titlePriseScore = "SCORE x3!"; // текст приза очков
	const string titlePriseSlice = "SLICE!"; // текст приза обрезки
	const string titlePriseSpeed = "SPEED!"; // текст приза скорости
	const Color priseScoreColor = Color(255, 255, 0); // цвет приза очков
	const Color priseSliceColor = Color(0, 255, 0); // цвет приза обрезки
	const Color priseSpeedColor = Color(0, 255, 255); // цвет приза скорости
	const Color backgroundColor = Color(32, 32, 32); // цвет фона
	const Color wallColor = Color(200, 0, 0); // цвет стен
	const Color foodColor = Color(0, 0, 200); // цвет еды дл€ змейки
	const Color snakeHeadColor = Color(0, 200, 0); // цвет головы змейки
	const Color snakeColor = Color(0, 150, 0); // цвет змейки
	const Color snakeHeadEndColor = Color(0, 100, 0); // цвет головы змейки, если ударилась
	const Color endCausedWallColor = Color(100, 0, 0); // цвет стены, в которую врезались
	const float stepPeriod = 0.3f; // шаг змейки происходит за это врем€ в секундах
	const float dtStepCoefficientDefault = 1.0f; // коэффициент времени по умолчанию
	const float priseSpeedMul = 2.0f; // коэффициент приза времени
	const float gameOverPeriod = 4.0f; // период моргани€ текста после завершени€ игры, нельз€ нажимать клавиши
	const float priseDisappearPeriod = 7.0f; // период исчезновени€ приза
	const float priseActivePeriod = 10.0f; // период активности приза

	const pair<uint8_t, uint8_t> wallsSize(1, 1); // толщина наружных стен
	const pair<uint8_t, uint8_t> gameInnerSize(16, 13); // внутренние размеры игрового пол€ (без стен)
	const uint8_t snakeFirstSize(3); // размер, с которым рождаетс€ змейка
	const Color endCausedSnakeColor = Color(0, 50, 0); // цвет тела змейки, если в него ударились
	const long priseScoreMul = 3; // коэффициент приза очков
	const int priseSliceCount = 1; // обрезаем змейку на столько €чеек после вз€ти€ приза
	const int priseSlicePerFood = 2; // обрезаем змейку каждый раз когда берет еду или приз во врем€ приза обрезки
	const int snakeMinSizeAfterSlice = 1; // минимальный размер змейки после обрезки
	const long scoreByFood = 10; // очков за еду
	const uint8_t foodCountBeforePrise = 2; // столько должны съесть еды, прежде чем по€витс€ приз
	const long scoreCoefficientDefault = 1; // коэффициент получаемых очков по умолчанию
}