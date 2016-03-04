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
	// ��������������� �������
	struct GameInfo {
		// ��������� ����
		string titleStart; // ��������� � ������ ����
		string titleEnd; // ��������� � ����� ����
		string titlePriseScore; // ����� ����� �����
		string titlePriseSlice; // ����� ����� �������
		string titlePriseSpeed; // ����� ����� ��������
		string fontId; // id ������
		Font* font; // ����� ����������� �����
		Color fontColor; // ���� ������
		Color priseScoreColor;// ���� ����� �����
		Color priseSliceColor; // ���� ����� �������
		Color priseSpeedColor; // ���� ����� ��������
		Color backgroundColor; // ���� ����
		Color wallColor; // ���� ����
		Color foodColor; // ���� ��� ��� ������
		Color snakeHeadColor; // ���� ������ ������
		Color snakeColor; // ���� ������
		Color snakeHeadEndColor; // ���� ������ ������, ���� ���������
		Color endCausedWallColor; // ���� �����, � ������� ���������
		Color endCausedSnakeColor; // ���� ������ ������, � ������� ���������
		float stepPeriod; // ��� ������ ���������� �� ��� ����� � ��������
		float dtStepCoefficientDefault; // ����������� ������� �� ���������
		float priseSpeedMul; // ����������� ����� �������
		float gameOverPeriod; // ������ �������� ������ ����� ���������� ����, ������ �������� �������
		float priseDisappearPeriod; // ������ ������������ �����
		float priseActivePeriod; // ������ ���������� �����

		// ���� ������
		pair<uint8_t, uint8_t> wallsSize; // ������� �������� ����
		pair<uint8_t, uint8_t> gameInnerSize; // ���������� ������� �������� ���� (��� ����)
		uint8_t snakeFirstSize; // ������, � ������� ��������� ������
		long priseScoreMul; // ����������� ����� �����
		int priseSliceCount; // �������� ������ �� ������� ����� ����� ������ �����
		int priseSlicePerFood; // �������� ������ ������ ��� ����� ����� ��� ��� ���� �� ����� ����� �������
		int snakeMinSizeAfterSlice; // ����������� ������ ������ ����� �������
		long scoreByFood; // ����� �� ���
		uint8_t foodCountBeforePrise; // ������� ������ ������ ���, ������ ��� �������� ����
		long scoreCoefficientDefault; // ����������� ���������� ����� �� ���������
		uint8_t priseSameTypeCountMax; // ������� ��� ������ ����� ���� ���� � ��� �� ��� �����

		Color ExtractXMLColor(XMLElement e); // �������� ���� �� xml
		bool ExtractXMLData(shared_ptr<XMLParser> parser); // ����������� ������ �� ���� �� xml �����
	};

	extern GameInfo gameInfo;

	
}