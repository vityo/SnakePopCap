#pragma once
#include <set>
#include <memory>
#include <queue>
#include "SexyAppFramework/Buffer.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/Point.h"

using namespace std;
using namespace Sexy;

namespace Game {
	// Игра змейка. Только дискретная логика
	class Snake {
	public:
		typedef shared_ptr<Snake> HardPtr; // сильный указатель
		typedef uint8_t index_type; // тип для индекса ячейки
		typedef pair<index_type, index_type> cell_type; // тип ячейки
		enum Direction{LEFT, RIGHT, UP, DOWN}; // типы направления движения
		enum State { READY, GAME, END }; // типы состояния игры
		enum PriseType { 
			SCORE = 0, // увеличиваем зарабатываемые очки
			SLICE = 1, // обрезаем змейку
			SPEED = 2, // изменяем скорость
			PRISE_COUNT = 3, // количество
			PRISE_NONE = 4 // пока не установлен никакой активный приз
		}; // возможные призы
		enum EventType {
			EVENT_NONE, // ничего не произошло
			EVENT_WALL, // ударились в стену
			EVENT_SNAKE, // ударились в змею
			EVENT_PRISE_CREATE, // создался приз
			EVENT_PRISE_EAT, // съели приз
			EVENT_FOOD // cъели еду
		}; // возможные события после хода змейки
		struct PriseCell {
			cell_type position; // позиция приза
			PriseType type; // тип приза
			PriseCell(cell_type _position, PriseType _type) : position(_position), type(_type) {}
		};

		Snake(cell_type wallsSize, set<cell_type> wallsInner); // конструктор. толщина наружных стен, и набор внутренних
		void start(); // начинаем игру
		EventType next(); // дискретный шаг змейки. true, если выполнилось движение
		void reset(); // сбросить игру, начать заново
		// так как мы сами по таймеру выбираем когда закончить действие приза:
		void priseActiveEnd(); // закончить действие активного приза
		void priseDisappear(); // удалить приз с поля

		State getState(); // узнать текущее состояние игры
		void setDirection(Direction direction); // установить возможное направление змейки
		pair<index_type, index_type> getGameSize(); // вернуть полные размеры игры
		cell_type getWallsSize(); // вернуть толщину наружных стен
		set<cell_type>& getWalls(); // вернуть ячейки со стенами
		deque<cell_type>& getSnake(); // вернуть ячейки змейки
		shared_ptr<cell_type> getFood(); // вернуть возможную ячейку еды
		shared_ptr<PriseCell> getPrise(); // вернуть приз на поле игры
		shared_ptr<cell_type> getСellEndCaused(); // вернуть ячейку, ставшую причиной смерти
		EventType getEventLast(); // узнать последнее событие
		long getScore(); // получить текущий счет
		PriseType getPriseTypeActive(); // получить активированный тип приза
	protected:
		void makeCellFreeWithoutWallContainer(); // делаем контейнер пустых клеток карты (не меняется в процессе игры) на основе стен 
		shared_ptr<cell_type> findCellFree(bool forPrise = false, bool withoutWallsOnly = false); // найти пустое свободное место
		void setPriseTypeActive(PriseType value); // установить тип активного приза
		void slice(int count); // обрезаем с хвоста

		cell_type _wallsSize; // толщина наружных стен
		set<cell_type> _walls; // внутренние стены
		set<cell_type> _cellsFreeWithoutWalls; // пустые клетки
		deque<cell_type> _snake; // ячейки змейки
		pair<int, int> _direction; // куда пойдет змейка дальше
		State _state; // состояние игры
		shared_ptr<cell_type> _food; // ячейка еды
		shared_ptr<cell_type> _cellEndCaused; // ячейка, ставшая причиной конца игры
		
		PriseType _priseTypeActive; // текущий тип активного приза
		uint8_t _foodCountBeforePrise; // текущее количество еды после последнего приза
		uint8_t _priseSameTypeCountLast; // сколько раз был один и тот же тип приза
		PriseType _priseTypeActiveLast; // последний тип приза, который был
		shared_ptr<PriseCell> _prise; // приз
		vector<Point> _priseTitlePositions; // позиции заголовков игры для соответствующих призов

		long _score; // текущий счет, очки
		pair<index_type, index_type> _gameSize; // полные размеры игры в координатах ячеек
		long _scoreCoefficient; // коэффициент получаемых очков
		EventType _eventLast; // последнее наступившее событие
	};
}