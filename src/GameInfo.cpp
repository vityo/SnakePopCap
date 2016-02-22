#include "stdafx.h"
#include "GameInfo.h"
#include "GameApp.h"
#include "SexyAppFramework/ResourceManager.h"

using namespace Sexy;

// TODO тут можно подробно расписать сообщения по ассертам

namespace Game {
	GameInfo gameInfo;

	Color GameInfo::ExtractXMLColor(XMLElement e) {
		int r = atoi(e.mAttributes[_S("r")].c_str());
		int g = atoi(e.mAttributes[_S("g")].c_str());
		int b = atoi(e.mAttributes[_S("b")].c_str());
		Assert(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255);

		return Color(r, g, b);
	}

	bool GameInfo::ExtractXMLData(shared_ptr<XMLParser> parser) {
		XMLElement e;

		while (parser->NextElement(&e)) {
			if (e.mType == XMLElement::TYPE_START) {
				if (e.mSection == _S("Interface")) {
					if (e.mValue == _S("titleStart")) {
						gameInfo.titleStart = e.mAttributes[_S("text")];
					}
					else if (e.mValue == _S("titleEnd")) {
						gameInfo.titleEnd = e.mAttributes[_S("text")];
					}
					else if (e.mValue == _S("titlePriseScore")) {
						gameInfo.titlePriseScore = e.mAttributes[_S("text")];
					}
					else if (e.mValue == _S("titlePriseSlice")) {
						gameInfo.titlePriseSlice = e.mAttributes[_S("text")];
					}
					else if (e.mValue == _S("titlePriseSpeed")) {
						gameInfo.titlePriseSpeed = e.mAttributes[_S("text")];
					}
					else if (e.mValue == _S("fontId")) {
						gameInfo.fontId = e.mAttributes[_S("text")];
						gameInfo.font = GameApp::instance()->mResourceManager->GetFontThrow(gameInfo.fontId);
					}
					else if (e.mValue == _S("fontColor")) {
						gameInfo.fontColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("priseScoreColor")) {
						gameInfo.priseScoreColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("priseSliceColor")) {
						gameInfo.priseSliceColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("priseSpeedColor")) {
						gameInfo.priseSpeedColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("backgroundColor")) {
						gameInfo.backgroundColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("wallColor")) {
						gameInfo.wallColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("foodColor")) {
						gameInfo.foodColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("snakeHeadColor")) {
						gameInfo.snakeHeadColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("snakeColor")) {
						gameInfo.snakeColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("snakeHeadEndColor")) {
						gameInfo.snakeHeadEndColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("endCausedWallColor")) {
						gameInfo.endCausedWallColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("endCausedSnakeColor")) {
						gameInfo.endCausedSnakeColor = ExtractXMLColor(e);
					}
					else if (e.mValue == _S("stepPeriod")) {
						gameInfo.stepPeriod = atof(e.mAttributes[_S("value")].c_str());
						Assert(gameInfo.stepPeriod > 0.f);
					}
					else if (e.mValue == _S("dtStepCoefficientDefault")) {
						gameInfo.dtStepCoefficientDefault = atof(e.mAttributes[_S("value")].c_str());
						Assert(gameInfo.dtStepCoefficientDefault > 0.f);
					}
					else if (e.mValue == _S("priseSpeedMul")) {
						gameInfo.priseSpeedMul = atof(e.mAttributes[_S("value")].c_str());
						Assert(gameInfo.priseSpeedMul > 0.f);
					}
					else if (e.mValue == _S("gameOverPeriod")) {
						gameInfo.gameOverPeriod = atof(e.mAttributes[_S("value")].c_str());
						Assert(gameInfo.gameOverPeriod > 0.f);
					}
					else if (e.mValue == _S("priseDisappearPeriod")) {
						gameInfo.priseDisappearPeriod = atof(e.mAttributes[_S("value")].c_str());
						Assert(gameInfo.priseDisappearPeriod > 0.f);
					}
					else if (e.mValue == _S("priseActivePeriod")) {
						gameInfo.priseActivePeriod = atof(e.mAttributes[_S("value")].c_str());
						Assert(gameInfo.priseActivePeriod > 0.f);
					}
				}
				else if (e.mSection == _S("Snake")) {
					if (e.mValue == _S("wallsSize")) {
						int width = atoi(e.mAttributes[_S("width")].c_str());
						int height = atoi(e.mAttributes[_S("height")].c_str());
						Assert(width > 0);
						Assert(height > 0);
						gameInfo.wallsSize = make_pair(width, height);
					}
					else if (e.mValue == _S("gameInnerSize")) {
						int width = atoi(e.mAttributes[_S("width")].c_str());
						int height = atoi(e.mAttributes[_S("height")].c_str());
						Assert(width > 0);
						Assert(height > 0);
						gameInfo.gameInnerSize = make_pair(width, height);
					}
					else if (e.mValue == _S("snakeFirstSize")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value > 0);
						gameInfo.snakeFirstSize = value;
					}
					else if (e.mValue == _S("priseScoreMul")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value > 0);
						gameInfo.priseScoreMul = value;
					}
					else if (e.mValue == _S("priseSliceCount")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value >= 0);
						gameInfo.priseSliceCount = value;
					}
					else if (e.mValue == _S("priseSlicePerFood")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value >= 0);
						gameInfo.priseSlicePerFood = value;
					}
					else if (e.mValue == _S("snakeMinSizeAfterSlice")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value > 0);
						gameInfo.snakeMinSizeAfterSlice = value;
					}
					else if (e.mValue == _S("scoreByFood")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value > 0);
						gameInfo.scoreByFood = value;
					}
					else if (e.mValue == _S("foodCountBeforePrise")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value > 0);
						gameInfo.foodCountBeforePrise = value;
					}
					else if (e.mValue == _S("scoreCoefficientDefault")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value > 0);
						gameInfo.scoreCoefficientDefault = value;
					}
					else if (e.mValue == _S("priseSameTypeCountMax")) {
						int value = atoi(e.mAttributes[_S("value")].c_str());
						Assert(value > 0);
						gameInfo.priseSameTypeCountMax = value;
					}
				}
			}
		}

		return true;
	}
}