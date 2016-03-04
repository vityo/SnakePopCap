#pragma once
#include <string>
#include <memory>
#include "SexyAppFramework/Color.h"
#include "SexyAppFramework/XMLParser.h"
#include "SexyAppFramework/ImageFont.h"
#include "SexyAppFramework/Color.h"

using namespace std;
using namespace Sexy;

namespace Game {
	// вспомогательные функции
	struct GameInfo {
		// интерфейс игры
		string titleStart; // заголовок в начале игры
		string titleEnd; // заголовок в конце игры
		string titlePriseScore; // текст приза очков
		string titlePriseSlice; // текст приза обрезки
		string titlePriseSpeed; // текст приза скорости
		string fontId; // id шрифта
		Font* font; // сразу загруженный шрифт
		Color fontColor; // цвет текста
		Color priseScoreColor;// цвет приза очков
		Color priseSliceColor; // цвет приза обрезки
		Color priseSpeedColor; // цвет приза скорости
		Color backgroundColor; // цвет фона
		Color wallColor; // цвет стен
		Color foodColor; // цвет еды дл€ змейки
		Color snakeHeadColor; // цвет головы змейки
		Color snakeColor; // цвет змейки
		Color snakeHeadEndColor; // цвет головы змейки, если ударилась
		Color endCausedWallColor; // цвет стены, в которую врезались
		Color endCausedSnakeColor; // цвет €чейки змейки, в которую врезались
		float stepPeriod; // шаг змейки происходит за это врем€ в секундах
		float dtStepCoefficientDefault; // коэффициент времени по умолчанию
		float priseSpeedMul; // коэффициент приза времени
		float gameOverPeriod; // период моргани€ текста после завершени€ игры, нельз€ нажимать клавиши
		float priseDisappearPeriod; // период исчезновени€ приза
		float priseActivePeriod; // период активности приза

		// сама змейка
		pair<uint8_t, uint8_t> wallsSize; // толщина наружных стен
		pair<uint8_t, uint8_t> gameInnerSize; // внутренние размеры игрового пол€ (без стен)
		uint8_t snakeFirstSize; // размер, с которым рождаетс€ змейка
		long priseScoreMul; // коэффициент приза очков
		int priseSliceCount; // обрезаем змейку на столько €чеек после вз€ти€ приза
		int priseSlicePerFood; // обрезаем змейку каждый раз когда берет еду или приз во врем€ приза обрезки
		int snakeMinSizeAfterSlice; // минимальный размер змейки после обрезки
		long scoreByFood; // очков за еду
		uint8_t foodCountBeforePrise; // столько должны съесть еды, прежде чем по€витс€ приз
		long scoreCoefficientDefault; // коэффициент получаемых очков по умолчанию
		uint8_t priseSameTypeCountMax; // сколько раз подр€д может быть один и тот же тип приза

		Color ExtractXMLColor(XMLElement e); // вытащить цвет из xml
		bool ExtractXMLData(shared_ptr<XMLParser> parser); // вытаскиваем данные об игре из xml файла
	};

	extern GameInfo gameInfo;

	
}