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
	// »гра змейка. “олько дискретна€ логика
	class Snake {
	public:
		typedef shared_ptr<Snake> HardPtr; // сильный указатель
		typedef uint8_t index_type; // тип дл€ индекса €чейки
		typedef pair<index_type, index_type> cell_type; // тип €чейки
		enum Direction{LEFT, RIGHT, UP, DOWN}; // типы направлени€ движени€
		enum State { READY, GAME, END }; // типы состо€ни€ игры
		enum PriseType { 
			SCORE = 0, // увеличиваем зарабатываемые очки
			SLICE = 1, // обрезаем змейку
			SPEED = 2, // измен€ем скорость
			PRISE_COUNT = 3, // количество
			PRISE_NONE = 4 // пока не установлен никакой активный приз
		}; // возможные призы
		enum EndCaused {
			CAUSED_NONE, // пока не установлено
			WALL, // ударились в стену
			SNAKE // ударились в змею
		}; // причина поражени€
		struct PriseCell {
			cell_type position; // позици€ приза
			PriseType type; // тип приза
			bool expired; // признак окончани€ работы приза
			PriseCell(cell_type _position, PriseType _type) : position(_position), type(_type), expired(false) {}
		};

		Snake(cell_type wallsSize, shared_ptr<Buffer> data); // конструктор. толщина стен
		void start(); // начинаем игру
		bool next(); // дискретный шаг змейки. true, если выполнилось движение
		void reset(); // сбросить игру, начать заново

		State getState(); // узнать текущее состо€ние игры
		void setDirection(Direction direction); // установить возможное направление змейки
		pair<index_type, index_type> getGameSize(); // вернуть полные размеры игры
		cell_type getWallsSize(); // вернуть толщину окружающих стен
		set<cell_type> getWalls(); // вернуть €чейки со стенами
		deque<cell_type> getSnake(); // вернуть €чейки змейки
		shared_ptr<cell_type> getFood(); // вернуть возможную €чейку еды
		shared_ptr<PriseCell> getPrise(); // вернуть приз на поле игры
		shared_ptr<cell_type> get—ellEndCaused(); // вернуть €чейку, ставшую причиной смерти
		EndCaused getEndCaused(); // узнать причину смерти
		long getScore(); // получить текущий счет
		PriseType getPriseTypeActive(); // получить активированный тип приза
	protected:
		void load(shared_ptr<Buffer> data); // загружаем карту из файла
		void makeCellFreeWithoutWallContainer(); // делаем контейнер пустых клеток карты (не мен€етс€ в процессе игры) на основе стен 
		shared_ptr<cell_type> findCellFree(bool forPrise = false, bool withoutWallsOnly = false); // найти пустое свободное место

		cell_type _wallsSize; // толщина стен
		set<cell_type> _walls; // стены
		set<cell_type> _cellsFreeWithoutWalls; // пустые клетки
		deque<cell_type> _snake; // €чейки змейки
		pair<int, int> _direction; // куда пойдет змейка дальше
		State _state; // состо€ние игры
		shared_ptr<cell_type> _food; // €чейка еды
		shared_ptr<cell_type> _cellEndCaused; // €чейка, ставша€ причиной конца игры
		
		PriseType _priseTypeActive; // текущий тип активного приза
		uint8_t _foodCountBeforePrise; // текущее количество еды после последнего приза
		shared_ptr<PriseCell> _prise; // приз
		vector<Point> _priseTitlePositions; // позиции заголовков игры дл€ соответствующих призов

		long _score; // текущий счет, очки
		pair<index_type, index_type> _gameSize; // полные размеры игры в координатах €чеек
		long _scoreCoefficient; // коэффициент получаемых очков
		EndCaused _endCaused; // причина поражени€
	};
}