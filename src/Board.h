#pragma once
#include "SexyAppFramework/Widget.h"
#include "Snake.h"
#include "SexyAppFramework/ImageFont.h"
#include <map>

namespace Sexy {
	class Graphics;
	class GameApp;

	// ������ �� �������
	class Board : public Widget {
	public:
		typedef shared_ptr<Board> HardPtr; // ������� ���������

		Board(int syncRefreshRate, pair<int, int> screenSize, shared_ptr<Buffer> data); // ��� �� ������� ��� ��������� � ������� ��� ���������
		virtual void UpdateF(float theFrac); // ����������
		virtual void Draw(Graphics* g); // ���������
		virtual void KeyDown(KeyCode theKey); // �������
	private:
		void reset(); // ���������� �������, ������
		set<Game::Snake::cell_type> load(shared_ptr<Buffer> data); // ��������� ����� �� �����

		Game::Snake::HardPtr _snake; // ������
		int _syncRefreshRate; // ������� ����������
		int _cellPixelSize; // ������ ������ � ������ � � ������
		Point _drawPosition; // ������� ��� ��������� ����� ���� ����
		vector<vector<Point>> _wallsPoly; // ������ ����� ��� ������������� ��������� ����������� ���� (��������, ������ ���� �����)
		// ��� ����������� ���������
		class Title {
		public:
			static void Init(Font* font, Color color); // ������������� ������ � ����� ������ ��� ����������
			Title(); // ����������� �� ���������
			Title(string caption, Point positionCenter); // �����������, ����� ��������� � ��� ����������� �������

			void setCaption(string value); // ���������� ����� ����� ���������, ��������������� ������� ���������
			string getCaption(); // ������� ����� ���������
			Point getPositionDraw(); // ������� ������� ���������
			void setPositionCenter(Point value); // ���������� ����������� ������� ������
			static Font* getFont(); // ����� ������
			static Color getColor(); // ���� ������
		private:
			void calcTitleShiftX(string _caption); // �������� ����� ��� ��������� �� ������ ��� ������, ������������ ������ �� ������

			Point _positionCenter; // ����������� ������� ������� ���������
			static Font* _font; // ����� ��� ������
			static Color _color; // ���� ������
			string _caption; // ����� ���������
			int _shiftX; // ����� ��������� � ������� ��� ���������
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
		pair<float, float> _titleTimer; // ������ �������� ������ ���������
		pair<float, float> _stepTimer; // ������ ��� ���� ������
		pair<float, float> _priseDisappearTimer; // ������ ������������ �����
		bool _titleDraw; // ��� �������� ���������
		bool _priseDraw; // ��� �������� �����
		float _dtStepCoefficient; // ����������� �������
	};
}