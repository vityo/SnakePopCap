#include "stdafx.h"
#include "Snake.h"
#include <algorithm>
#include <iterator>
#include "Utils.h"
#include "GameInfo.h"

using namespace Game;

Snake::Snake(cell_type wallsSize, set<cell_type> wallsInner)
	: _wallsSize(wallsSize)
	, _walls(wallsInner)
{ 
	_gameSize = make_pair(gameInfo.gameInnerSize.first + wallsSize.first * 2, gameInfo.gameInnerSize.second + wallsSize.second * 2);
	makeCellFreeWithoutWallContainer();
	reset();
}

void Snake::reset() {
	_priseTypeActiveLast = PRISE_NONE;
	_priseSameTypeCountLast = 0;
	_eventLast = EVENT_NONE;
	_priseTypeActive = PRISE_NONE;
	_scoreCoefficient = gameInfo.scoreCoefficientDefault;
	_foodCountBeforePrise = 0;
	_score = 0;
	_cellEndCaused = nullptr;

	// выбираем место дл€ старта змейки
	shared_ptr<cell_type> snakeStart = make_shared<cell_type>(_wallsSize.first + static_cast<index_type>(gameInfo.gameInnerSize.first * 0.5f),
		_wallsSize.second + static_cast<index_type>(gameInfo.gameInnerSize.second * 0.5f));
	set<cell_type>::iterator snakeInWallIt = _walls.find(*snakeStart);

	if (snakeInWallIt != _walls.end()) {
		shared_ptr<cell_type> cellFree = findCellFree(false, true);
		
		if(cellFree) {
			snakeStart = cellFree;
		}
		else {
			_walls.erase(snakeInWallIt);
		}
	}

	_snake.clear();
	_snake.resize(gameInfo.snakeFirstSize);
	fill_n(_snake.begin(), gameInfo.snakeFirstSize, *snakeStart);
	setDirection(RIGHT);
	_state = READY;
	_food = findCellFree();
	_prise = nullptr;
}

pair<Snake::index_type, Snake::index_type> Snake::getGameSize() {
	return _gameSize;
}

Snake::cell_type Snake::getWallsSize() {
	return _wallsSize;
}

set<Snake::cell_type>& Snake::getWalls() {
	return _walls;
}

deque<Snake::cell_type>& Snake::getSnake() {
	return _snake;
}

shared_ptr<Snake::cell_type> Snake::getFood() {
	return _food;
}

shared_ptr<Snake::PriseCell> Snake::getPrise() {
	return _prise;
}

shared_ptr<Snake::cell_type> Snake::get—ellEndCaused() {
	return _cellEndCaused;
}

Snake::EventType Snake::getEventLast() {
	return _eventLast;
}

long Snake::getScore() {
	return _score;
}

Snake::PriseType Snake::getPriseTypeActive() {
	return _priseTypeActive;
}

shared_ptr<Snake::cell_type> Snake::findCellFree(bool forPrise, bool withoutWallsOnly) {
	shared_ptr<cell_type> cellFree = nullptr;
	set<cell_type> cellsFree;

	if (forPrise) {
		withoutWallsOnly = false;
	}

	if (withoutWallsOnly) {
		cellsFree = _cellsFreeWithoutWalls;
	}
	else {
		set<cell_type> cellsFreeWithoutWalls = _cellsFreeWithoutWalls;
		set<cell_type> snakeAndFoodAndPrise;
		copy(_snake.begin(), _snake.end(), inserter(snakeAndFoodAndPrise, snakeAndFoodAndPrise.end()));

		if (forPrise) {
			snakeAndFoodAndPrise.insert(cell_type(_food->first, _food->second));
		}
		else if (_prise) {
			snakeAndFoodAndPrise.insert(cell_type(_prise->position.first, _prise->position.second));
		}

		// из множества пустых €чеек внутриигровой области вычитаем €чейки змейки и еды
		set_difference(cellsFreeWithoutWalls.begin(), cellsFreeWithoutWalls.end(), 
			snakeAndFoodAndPrise.begin(), snakeAndFoodAndPrise.end(), inserter(cellsFree, cellsFree.end()));
	}

	// в cellsFree €чейки, из которых можем выбрать
	if (!cellsFree.empty()) {
		vector<cell_type>::size_type cellFreeIndex = utils::Math::random(0, cellsFree.size() - 1);;
		auto cellIt = cellsFree.begin();
		advance(cellIt, cellFreeIndex);
		cellFree = make_shared<cell_type>(cellIt->first, cellIt->second);
	}

	return cellFree;
}

void Snake::makeCellFreeWithoutWallContainer() {
	_cellsFreeWithoutWalls.clear();
	set<cell_type> cellsFree;

	for (index_type i = _wallsSize.first; i < _wallsSize.first + gameInfo.gameInnerSize.first; ++i)
		for (index_type j = _wallsSize.second; j < _wallsSize.second + gameInfo.gameInnerSize.second; ++j) {
			cellsFree.insert(cell_type(i, j));
		}

	// из множества €чеек внутриигровой области вычитаем €чейки стен внутриигровой области
	set_difference(cellsFree.begin(), cellsFree.end(), _walls.begin(), _walls.end(), 
		inserter(_cellsFreeWithoutWalls, _cellsFreeWithoutWalls.end()));
}

Snake::State Snake::getState() {
	return _state;
}

void Snake::setDirection(Direction direction) {
	pair<int, int> directionNew;

	switch (direction) {
	case LEFT:
		directionNew = make_pair(-1, 0);
		break;
	case RIGHT:
		directionNew = make_pair(1, 0);
		break;
	case UP:
		directionNew = make_pair(0, -1);
		break;
	case DOWN:
		directionNew = make_pair(0, 1);
		break;
	}

	cell_type cell = _snake.back();
	cell.first += directionNew.first;
	cell.second += directionNew.second;

	if (_snake.size() == 1 // если осталась только голова, то пусть двигаетс€ куда хочет
		|| cell != *(++_snake.rbegin())) {
		_direction = directionNew;
	}
}

void Snake::start() {
	if (_state == READY) {
		_state = GAME;
	}
}

void Snake::priseActiveEnd() {
	_priseTypeActive = PRISE_NONE;
	_scoreCoefficient = gameInfo.scoreCoefficientDefault;
}

void Snake::priseDisappear() {
	_prise = nullptr;
}

void Snake::slice(int count) {
	if (_snake.size() > gameInfo.snakeMinSizeAfterSlice) {
		int sliceCount = _snake.size() - max(gameInfo.snakeMinSizeAfterSlice, static_cast<int>(_snake.size()) - count);

		while (sliceCount--) {
			_snake.pop_front();
		}
	}
}

void Snake::setPriseTypeActive(PriseType value) {
	priseActiveEnd();
	_priseTypeActive = value;

	if (_priseTypeActive == SCORE) {
		_scoreCoefficient = gameInfo.priseScoreMul;
	}
	else if (_priseTypeActive == SLICE) {
		slice(gameInfo.priseSliceCount);
	}

	if (_priseTypeActiveLast == value) {
		++_priseSameTypeCountLast;
	}
	else {
		_priseSameTypeCountLast = 1;
		_priseTypeActiveLast = value;
	}
}

Snake::EventType Snake::next() {
	if (_state != END) {
		cell_type cell = _snake.back();
		cell.first += _direction.first;
		cell.second += _direction.second;

		bool isEndByWall = binary_search(_walls.begin(), _walls.end(), cell)
			|| cell.first < _wallsSize.first
			|| cell.first > _wallsSize.first + gameInfo.gameInnerSize.first - 1
			|| cell.second < _wallsSize.second
			|| cell.second > _wallsSize.second + gameInfo.gameInnerSize.second - 1;
		bool isEndBySnake = find(++_snake.begin(), _snake.end(), cell) != _snake.end();
		bool isEnd = isEndByWall || isEndBySnake;

		if (isEnd) {
			// во что-то ударились, завершаем игру
			_eventLast = isEndByWall ? EVENT_WALL : EVENT_SNAKE;
			_cellEndCaused = make_shared<cell_type>(cell.first, cell.second);
			_state = END;
		}
		else {
			// игра продолжаетс€
			_snake.push_back(cell);

			bool isFood = _food && cell.first == _food->first && cell.second == _food->second;
			bool isPrise = _prise && cell.first == _prise->position.first && cell.second == _prise->position.second;

			if (isFood) {
				// съели еду
				_eventLast = EVENT_FOOD;
				_food = findCellFree();
				++_foodCountBeforePrise;

				if (_foodCountBeforePrise == gameInfo.foodCountBeforePrise) {
					_foodCountBeforePrise = 0;
					shared_ptr<cell_type> cell = findCellFree(true);

					//только если есть место
					if (cell) {
						_eventLast = EVENT_PRISE_CREATE;

						PriseType priseType;

						do {
							priseType = static_cast<PriseType>(utils::Math::random(0, PRISE_COUNT - 1));
						} while ((_priseTypeActiveLast == priseType && _priseSameTypeCountLast + 1 > gameInfo.priseSameTypeCountMax));
						
						_prise = make_shared<PriseCell>(*cell.get(), priseType);
					}
				}
			}
			else if (isPrise) {
				// съели приз
				_eventLast = EVENT_PRISE_EAT;
				setPriseTypeActive(_prise->type);
				_prise = nullptr;
			}
			else {
				_eventLast = EVENT_NONE;
				_snake.pop_front();
			}

			// сейчас уже установили возможный съеденный тип приза
			if (isFood || isPrise) {
				if (_priseTypeActive == SLICE) {
					slice(gameInfo.priseSlicePerFood);
				}

				_score += gameInfo.scoreByFood * _scoreCoefficient;
			}
		}
	}

	return _eventLast;
}