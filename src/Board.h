#pragma once
#include "SexyAppFramework/Widget.h"
#include "Snake.h"
#include <map>

namespace Sexy {
	class Graphics;
	class GameApp;

	// виджет со змейкой
	class Board : public Widget {
	public:
		typedef shared_ptr<Board> HardPtr; // сильный указатель

		Board(int syncRefreshRate, pair<int, int> screenSize, shared_ptr<Buffer> data); // тут же считаем все параметры и размеры дл€ отрисовки
		virtual void UpdateF(float theFrac); // обновление
		virtual void Draw(Graphics* g); // отрисовка
		virtual void KeyDown(KeyCode theKey); // нажати€
	private:
		void reset(); // сбрасывает таймеры, тексты
		set<Game::Snake::cell_type> load(shared_ptr<Buffer> data); // загружаем карту из файла

		Game::Snake::HardPtr _snake; // змейка
		int _syncRefreshRate; // частота обновлени€
		int _cellPixelSize; // размер €чейки в ширину и в высоту
		Point _drawPosition; // позици€ дл€ отрисовки всего пол€ игры
		vector<vector<Point>> _wallsPoly; // наборы точек дл€ полигональной отрисовки статических стен (например, вокруг всей карты)
		struct Title {
		public:
			static void Init(Font* font);
			Title();
			Title(string caption, Point positionCenter);

			void setCaption(string value);
			string getCaption();
			Point getPositionDraw();
			void setPositionCenter(Point value);
			static Font* getFont();
		private:
			void calcTitleShiftX(string _caption); // получить сдвиг дл€ заголовка на основе его текста, выравнивание текста по центру

			Point _positionCenter; // центральна€ позици€ пустого заголовка
			static Font* _font; // шрифт дл€ текста
			string _caption; // текст заголовка
			int _shiftX; // сдвиг заголовка с текстом дл€ отрисовки
		};
		struct PriseParameters {
			Title title; // заголовок дл€ приза
			Color color; // цвет €чейки приза
			PriseParameters(Title _title, Color _color) : title(_title), color(_color) {}
		};
		map<Game::Snake::PriseType, PriseParameters> _priseParameters; // соответствие типу приза из змейки и визуальным параметрам в игре
		map<Game::Snake::State, Title> _stateParameters; // соответствие состо€нию из змейки и заголовкам в игре
		Title _title; // текущий заголовок
		Title _titleScore; // текст заработанных очков
		
		Color _cellEndCausedColor; // цвет дл€ отрисовки €чейки, ставшей причиной конца игры
		pair<float, float> _titleTimer; // таймер моргани€ текста заголовка
		pair<float, float> _stepTimer; // таймер дл€ шага змейки
		pair<float, float> _priseDisappearTimer; // таймер исчезновени€ приза
		bool _titleDraw; // дл€ моргани€ заголовка
		bool _priseDraw; // дл€ моргани€ приза
		float _dtStepCoefficient; // коэффициент времени
	};
}