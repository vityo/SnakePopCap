#include "stdafx.h"
#include "Snake.h"
#include <algorithm>
#include <iterator>
#include "SexyAppFramework/Color.h"
#include "SexyAppFramework/Point.h"
#include "Utils.h"

using namespace Game;

const Snake::cell_type gameInnerSize(16, 13); // внутренние размеры игрового пол€ (без стен)
const uint8_t snakeFirstSize(3); // размер, с которым рождаетс€ змейка
const Color endCausedSnakeColor = Color(0, 50, 0); // цвет тела змейки, если в него ударились
const long priseScoreMul = 3; // коэффициент приза очков
const long priseSliceCount = 5; // обрезаем змейку на столько €чеек после вз€ти€ приза
const long scoreByFood = 10; // очков за еду
const uint8_t foodCountBeforePrise = 2; // столько должны съесть еды, прежде чем по€витс€ приз
const long scoreCoefficientDefault = 1; // коэффициент получаемых очков по умолчанию

Snake::Snake(cell_type wallsSize, shared_ptr<Buffer> data)
	: _wallsSize(wallsSize)
{ 
	_gameSize = make_pair(gameInnerSize.first + wallsSize.first * 2, gameInnerSize.second + wallsSize.second * 2);
	load(data);
	makeCellFreeWithoutWallContainer();
	reset();
}

void Snake::reset() {
	_endCaused = CAUSED_NONE;
	_priseTypeActive = PRISE_NONE;
	_scoreCoefficient = scoreCoefficientDefault;
	_foodCountBeforePrise = 0;
	_score = 0;
	_cellEndCaused = nullptr;

	// выбираем место дл€ старта змейки
	shared_ptr<cell_type> snakeStart = make_shared<cell_type>(_wallsSize.first + static_cast<index_type>(gameInnerSize.first * 0.5f),
		_wallsSize.second + static_cast<index_type>(gameInnerSize.second * 0.5f));
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
	_snake.resize(snakeFirstSize);
	fill_n(_snake.begin(), snakeFirstSize, *snakeStart);
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

set<Snake::cell_type> Snake::getWalls() {
	return _walls;
}

deque<Snake::cell_type> Snake::getSnake() {
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

Snake::EndCaused Snake::getEndCaused() {
	return _endCaused;
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
		set<cell_type> snakeAndFood;
		copy(_snake.begin(), _snake.end(), inserter(snakeAndFood, snakeAndFood.end()));

		if (forPrise) {
			snakeAndFood.insert(cell_type(_food->first, _food->second));
		}

		// из множества пустых €чеек внутриигровой области вычитаем €чейки змейки и еды
		set_difference(cellsFreeWithoutWalls.begin(), cellsFreeWithoutWalls.end(), snakeAndFood.begin(), snakeAndFood.end(), inserter(cellsFree, cellsFree.end()));
	}

	// в cellsFree €чейки, из которых можем выбрать
	if (cellsFree.size()) {
		vector<cell_type>::size_type cellFreeIndex = utils::Math::random(0, cellsFree.size() - 1);;
		auto cellIt = cellsFree.begin();
		advance(cellIt, cellFreeIndex);
		cellFree = make_shared<cell_type>(cellIt->first, cellIt->second);
	}

	return cellFree;
}

void Snake::load(shared_ptr<Buffer> data) {
	// читаем файл карты со стенами. по максимуму используем функции PopCap, которые работают, ничего в нем не мен€€. Ќапример, ReadBuffer, ReadString - уже падают
	ByteVector byteVector;
	const uint8_t symbols = 2; // перенос строки и возврат каретки
	int dataLength = (gameInnerSize.first + symbols) * gameInnerSize.second - symbols;
	int dataLengthActual = data->GetDataLen();
	string message = "Invalid card file. Card file should be " + to_string(gameInnerSize.first) 
		+ "x" + to_string(gameInnerSize.second) + " size and contain non-whitespace characters in place of walls";
	Assert2(dataLength == dataLengthActual, message.c_str());
	byteVector.assign(dataLength, (uchar)0);
	data->ReadBytes(&byteVector[0], dataLengthActual);

	uint8_t wallXIndex = _wallsSize.first;
	uint8_t wallYIndex = _wallsSize.second;

	for (uchar ch : byteVector) {
		if (ch != '\n' && ch != '\r') {
			if (ch != ' ') {
				_walls.insert(cell_type(wallXIndex, wallYIndex));
			}

			++wallXIndex;

			if (wallXIndex == gameInnerSize.first + _wallsSize.first) {
				++wallYIndex;
				wallXIndex = _wallsSize.first;
			}
		}
	}
}

void Snake::makeCellFreeWithoutWallContainer() {
	_cellsFreeWithoutWalls.clear();
	set<cell_type> cellsFree;

	for (index_type i = _wallsSize.first; i < _wallsSize.first + gameInnerSize.first; ++i)
		for (index_type j = _wallsSize.second; j < _wallsSize.second + gameInnerSize.second; ++j) {
			cellsFree.insert(cell_type(i, j));
		}

	// из множества €чеек внутриигровой области вычитаем €чейки стен внутриигровой области
	set_difference(cellsFree.begin(), cellsFree.end(), _walls.begin(), _walls.end(), inserter(_cellsFreeWithoutWalls, _cellsFreeWithoutWalls.end()));
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

bool Snake::next() {
	bool isEnd = _state == END;

	if (!isEnd) {
		if (_prise && _prise->expired) {
			_prise = nullptr;
			_priseTypeActive = PRISE_NONE;
		}

		cell_type cell = _snake.back();
		cell.first += _direction.first;
		cell.second += _direction.second;

		bool isEndByWall = _walls.find(cell) != _walls.end()
			|| cell.first < _wallsSize.first
			|| cell.first > _wallsSize.first + gameInnerSize.first - 1
			|| cell.second < _wallsSize.second
			|| cell.second > _wallsSize.second + gameInnerSize.second - 1;
		bool isEndBySnake = find(++_snake.begin(), _snake.end(), cell) != _snake.end();
		isEnd = isEndByWall || isEndBySnake;

		if (isEnd) {
			// во что-то ударились, завершаем игру
			_cellEndCaused = make_shared<cell_type>(cell.first, cell.second);
			_state = END;
			_endCaused = isEndByWall ? WALL : SNAKE;
		}
		else {
			// игра продолжаетс€
			_snake.push_back(cell);

			if (_food && cell.first == _food->first && cell.second == _food->second) {
				// съели еду
				_food = findCellFree();
				_score += scoreByFood * _scoreCoefficient;
				++_foodCountBeforePrise;

				if (_foodCountBeforePrise == foodCountBeforePrise) {
					_foodCountBeforePrise = 0;
					shared_ptr<cell_type> cell = findCellFree(true);

					//только если есть место
					if (cell) {
						_prise = make_shared<PriseCell>(*cell.get(), static_cast<PriseType>(utils::Math::random(0, PRISE_COUNT - 1)));
					}
				}
			}
			else if (_prise && cell.first == _prise->position.first && cell.second == _prise->position.second) {
				// съели приз
				_priseTypeActive = _prise->type;
				_prise = nullptr;
				_scoreCoefficient = scoreCoefficientDefault;

				if (_priseTypeActive == SCORE) {
					_scoreCoefficient = priseScoreMul;
				}
				else if (_priseTypeActive == SLICE) {
					int sliceCount = _snake.size() - max(1, _snake.size() - priseSliceCount);

					while (sliceCount--) {
						_snake.pop_front();
					}
				}
			}
			else {
				_snake.pop_front();
			}
		}
	}

	return !isEnd;
}