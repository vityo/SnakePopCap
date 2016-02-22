#pragma once
#include "SexyAppFramework/Widget.h"
#include "Snake.h"
#include <map>

namespace Sexy {
	class Graphics;
	class GameApp;

	// ������ �� �������
	class Board : public Widget {
	public:
		typedef shared_ptr<Board> HardPtr; // ������� ���������

		Board(Game::Snake::HardPtr snake, int syncRefreshRate, pair<int, int> screenSize); // ��� �� ������� ��� ��������� � ������� ��� ���������
		virtual void UpdateF(float theFrac); // ����������
		virtual void Draw(Graphics* g); // ���������
		virtual void KeyDown(KeyCode theKey); // �������
	private:
		Game::Snake::HardPtr _snake; // ������
		int _syncRefreshRate; // ������� ����������
		int _cellPixelSize; // ������ ������ � ������ � � ������
		Point _drawPosition; // ������� ��� ��������� ����� ���� ����
		vector<vector<Point>> _wallsPoly; // ������ ����� ��� ������������� ��������� ����������� ���� (��������, ������ ���� �����)
		struct Title {
		public:
			static void Init(Font* font, Point positionCenter);
			Title();
			Title(string caption);

			void setCaption(string value);
			string getCaption();
			Point getPosition();
			void setPosition(Point value);
			static Font* getFont();
		private:
			Point getTitlePosition(string _caption); // �������� ������� ��� ��������� �� ������ ��� ������

			static Point _positionCenter; // ����������� ������� ������� ���������
			static Font* _font; // ����� ��� ������
			string _caption; // ����� ���������
			Point _position; // ������� ��������� � �������
		};
		struct PriseParameters {
			Title title; // ��������� ��� �����
			Color color; // ���� ������ �����
			PriseParameters(Title _title, Color _color) : title(_title), color(_color) {}
		};
		map<Game::Snake::PriseType, PriseParameters> _priseParameters; // ������������ ���� ����� �� ������ � ���������� ���������� � ����
		map<Game::Snake::State, Title> _stateParameters; // ������������ ��������� �� ������ � ���������� � ����
		Title _title; // ������� ���������
		Title _titleScore; // ����� ������������ �����
		
		Color _cellEndCausedColor; // ���� ��� ��������� ������, ������� �������� ����� ����
		pair<float, float> _gameOverTimer; // ������ �������� ������ ��������� ��� ����� ����
		pair<float, float> _stepTimer; // ������ ��� ���� ������
		pair<float, float> _priseTimer; // ������ ������������ �����
		bool _titleDraw; // ��� �������� ���������
		bool _priseDraw; // ��� �������� �����
		float _dtStepCoefficient; // ����������� �������
	};
}