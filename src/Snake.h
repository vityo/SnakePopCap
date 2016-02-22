#pragma once
#include <set>
#include <memory>
#include <queue>
#include "SexyAppFramework/Buffer.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/Point.h"
#include "SexyAppFramework/ImageFont.h"

using namespace std;
using namespace Sexy;

namespace Game {
	// Игра змейка
	class Snake {
	public:
		typedef shared_ptr<Snake> HardPtr; // сильный указатель
		typedef uint8_t index_type; // тип для индекса ячейки
		typedef pair<index_type, index_type> cell_type; // тип ячейки
		enum Direction{LEFT, RIGHT, UP, DOWN}; // типы направления движения
		enum State { READY, GAME, END }; // типы состояния игры

		Snake(shared_ptr<Buffer> data, pair<int, int> screenSize); // конструктор, буффер с данными карты и размеры экрана
		virtual void Update(float dt); // dt прирост времени с последнего кадра
		virtual void Draw(Graphics* g); // отрисовка

		State getState(); // узнать текущее состояние игры
		void setState(State state); // установить текущее состояние игры
		void setDirection(Direction direction); // установить возможное направление змейки
		void reset(); // сбросить игру, начать заново
		bool isGameOverTimer(); // игра реально закончилась, но еще моргает заголовок + стоит подождать, пока игрок поймет, что проиграл
	protected:
		typedef uint8_t pixel_type; // тип для измерения пикселей (ячейки небольшие, поэтому такой)
		enum Prise { 
			SCORE = 0, // увеличиваем зарабатываемые очки
			SLICE = 1, // обрезаем змейку
			SPEED = 2, // изменяем скорость
			PRISE_COUNT = 3 // количество
		}; // возможные призы

		void load(shared_ptr<Buffer> data); // загружаем карту из файла
		void makeCellFreeWithoutWallContainer(); // делаем контейнер пустых клеток карты (не меняется в процессе игры) на основе стен 
		Point getTitlePosition(string title); // получить позицию для заголовка на основе его текста
		void calcDrawSizes(pair<int, int> screenSize); // посчитать все размеры для отрисовки
		void next(); // дискретный шаг змейки
		void setScore(long value); // установить количество заработанных очков
		void runPrise(Prise type); // запустить то, что дает приз
		shared_ptr<cell_type> findCellFree(bool forPrise = false, bool withoutWallsOnly = false); // найти пустое свободное место

		set<cell_type> _walls; // стены
		set<cell_type> _cellsFreeWithoutWalls; // пустые клетки
		vector<vector<Point>> _wallsPoly; // наборы точек для полигональной отрисовки статических стен (например, вокруг всей карты)
		deque<cell_type> _snake; // ячейки змейки
		pixel_type _cellPixelSize; // размер ячейки в ширину и в высоту
		Point _drawPosition; // позиция для отрисовки всего поля игры
		pair<int, int> _direction; // куда пойдет змейка дальше
		pair<float, float> _stepTimer; // таймер для шага змейки
		State _state; // состояние игры
		Font* _font; // шрифт для текста
		int _fontHeight; // высота шрифта
		bool _titleDraw; // для моргания заголовка
		string _title; // текст заголовка
		Point _titlePosition; // позиция заголовка
		Point _titleStartPosition; // позиция заголовка начального экрана
		Point _titleEndPosition; // позиция заголовка в конце игры
		shared_ptr<cell_type> _food; // ячейка еды
		shared_ptr<cell_type> _cellEndCaused; // ячейка, ставшая причиной конца игры
		Color _cellEndCausedColor; // цвет для отрисовки ячейки, ставшей причиной конца игры
		
		Prise _priseType; // тип приза
		Color _priseColor; // цвет отрисовки приза
		uint8_t _foodCountBeforePrise; // текущее количество еды после последнего приза
		shared_ptr<cell_type> _prise; // ячейка приза
		vector<Point> _priseTitlePositions; // позиции заголовков игры для соответствующих призов
		pair<float, float> _priseTimer; // таймер исчезновения приза
		bool _priseDraw; // для моргания приза

		long _score; // текущий счет, очки
		Point _scorePosition; // позиция для отрисовки текста очков
		pair<index_type, index_type> _gameSize; // полные размеры игры в координатах ячеек
		float _dtStepCoefficient; // коэффициент времени
		long _scoreCoefficient; // коэффициент получаемых очков
		pair<float, float> _gameOverTimer; // таймер моргания текста заголовка для конца игры
	};
}