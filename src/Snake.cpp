#include "stdafx.h"
#include "Snake.h"
#include <algorithm>
#include <iterator>
#include "GameApp.h"
#include "SexyAppFramework/Color.h"
#include "SexyAppFramework/Point.h"
#include "SexyAppFramework/ResourceManager.h"
#include "Utils.h"

using namespace Game;

const Snake::cell_type gameInnerSize(16, 13);
const Snake::cell_type wallsSize(1, 1);
const uint8_t snakeFirstSize(3);
const float stepPeriod = 0.3f;
const Color wallColor = Color(200, 0, 0);
const Color snakeColor = Color(0, 150, 0);
const Color snakeHeadColor = Color(0, 200, 0);
const Color snakeHeadEndColor = Color(0, 100, 0);
const Color endCausedWallColor = Color(100, 0, 0);
const Color endCausedSnakeColor = Color(0, 50, 0);
const Color fontColor = Color(255, 255, 255);
const Color foodColor = Color(0, 0, 200);
const string titleStart = "Press any arrow keys to start!";
const string titleEnd = "GAME OVER. Press any key to start again!";
const string fontId = "FONT_DEFAULT";
const long scoreByFood = 10;

Snake::Snake(shared_ptr<Buffer> data, pair<int, int> screenSize)
	: _cellEndCausedColor(endCausedWallColor)
{ 
	_font = GameApp::instance()->mResourceManager->GetFontThrow(fontId);
	load(data);
	makeCellFreeWithoutWallContainer();
	calcDrawSizes(screenSize);
	reset();
}

void Snake::reset() {
	setScore(0);
	_cellEndCaused = nullptr;
	shared_ptr<cell_type> snakeStart = make_shared<cell_type>(wallsSize.first + static_cast<index_type>(gameInnerSize.first * 0.5f),
		wallsSize.second + static_cast<index_type>(gameInnerSize.second * 0.5f));
	set<cell_type>::iterator snakeInWallIt = _walls.find(*snakeStart);

	if (snakeInWallIt != _walls.end()) {
		shared_ptr<cell_type> cellFree = findCellFree(true);
		
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
	_stepTimer = make_pair(0.f, stepPeriod);
	setDirection(RIGHT);
	setState(READY);
	_food = findCellFree();
}

shared_ptr<Snake::cell_type> Snake::findCellFree(bool withoutWallsOnly) {
	shared_ptr<cell_type> cellFree = nullptr;
	set<cell_type> cellsFree;

	if (withoutWallsOnly) {
		cellsFree = _cellsFreeWithoutWalls;
	}
	else {
		set<cell_type> cellsFreeWithoutWalls = _cellsFreeWithoutWalls;
		// сюда добавить призы текущие!!!

		set<cell_type> snake;
		copy(_snake.begin(), _snake.end(), inserter(snake, snake.end()));
		set_difference(cellsFreeWithoutWalls.begin(), cellsFreeWithoutWalls.end(), snake.begin(), snake.end(), inserter(cellsFree, cellsFree.end()));
	}

	if (cellsFree.size()) {
		vector<cell_type>::size_type cellFreeIndex = utils::random(0, cellsFree.size() - 1);;
		auto cellIt = cellsFree.begin();
		advance(cellIt, cellFreeIndex);
		cellFree = make_shared<cell_type>(cellIt->first, cellIt->second);
		cellsFree.erase(cellIt);
	}

	return cellFree;
}

void Snake::calcDrawSizes(pair<int, int> screenSize) {
	float screenAspect = static_cast<float>(screenSize.first) / static_cast<float>(screenSize.second);
	_gameSize = make_pair(gameInnerSize.first + wallsSize.first * 2, gameInnerSize.second + wallsSize.second * 2);
	float gameAspect = static_cast<float>(_gameSize.first) / static_cast<float>(_gameSize.second);
	_cellPixelSize = screenAspect > gameAspect ? static_cast<pixel_type>(static_cast<float>(screenSize.second) / static_cast<float>(_gameSize.second))
			: static_cast<pixel_type>(static_cast<float>(screenSize.first) / static_cast<float>(_gameSize.first));
	_drawPosition = Point(static_cast<int>((screenSize.first - _cellPixelSize * _gameSize.first) * 0.5f),
		static_cast<int>((screenSize.second - _cellPixelSize * _gameSize.second) * 0.5f));

	_wallsPoly = { { Point(0, 0),
		Point(0, _gameSize.second * _cellPixelSize),
		Point(_gameSize.first * _cellPixelSize, _gameSize.second * _cellPixelSize),
		Point(_gameSize.first * _cellPixelSize, (_gameSize.second - wallsSize.second) * _cellPixelSize),
		Point(wallsSize.first * _cellPixelSize, (_gameSize.second - wallsSize.second) * _cellPixelSize),
		Point(wallsSize.first * _cellPixelSize, 0) },
		{ Point(wallsSize.first * _cellPixelSize, 0),
		Point(wallsSize.first * _cellPixelSize, wallsSize.second * _cellPixelSize),
		Point((_gameSize.first - wallsSize.first) * _cellPixelSize, wallsSize.second * _cellPixelSize),
		Point((_gameSize.first - wallsSize.first) * _cellPixelSize, (_gameSize.second - wallsSize.second) * _cellPixelSize),
		Point(_gameSize.first * _cellPixelSize, (_gameSize.second - wallsSize.second) * _cellPixelSize),
		Point(_gameSize.first * _cellPixelSize, 0)}};

	int fontHeight = _font->GetHeight();
	_titleStartPosition = Point((_gameSize.first * _cellPixelSize - _font->StringWidth(titleStart)) / 2, static_cast<int>(_cellPixelSize - fontHeight) );
	_titleEndPosition = Point((_gameSize.first * _cellPixelSize - _font->StringWidth(titleEnd)) / 2, static_cast<int>(_cellPixelSize - fontHeight));
	_scorePosition.mY = static_cast<int>(_gameSize.second * _cellPixelSize - fontHeight);
}

void Snake::load(shared_ptr<Buffer> data) {
	// читаем файл карты со стенами. по максимуму используем функции PopCap, которые работают, ничего в нем не меняя. Например, ReadBuffer, ReadString - уже падают
	ByteVector byteVector;
	const uint8_t symbols = 2; // перенос строки и возврат каретки
	int dataLength = (gameInnerSize.first + symbols) * gameInnerSize.second - symbols;
	int dataLengthActual = data->GetDataLen();
	string message = "Invalid card file. Card file should be " + to_string(gameInnerSize.first) 
		+ "x" + to_string(gameInnerSize.second) + " size and contain non-whitespace characters in place of walls";
	Assert2(dataLength == dataLengthActual, message.c_str());
	byteVector.assign(dataLength, (uchar)0);
	data->ReadBytes(&byteVector[0], dataLengthActual);

	uint8_t wallXIndex = wallsSize.first;
	uint8_t wallYIndex = wallsSize.second;

	for (uchar ch : byteVector) {
		if (ch != '\n' && ch != '\r') {
			if (ch != ' ') {
				_walls.insert(cell_type(wallXIndex, wallYIndex));
			}

			++wallXIndex;

			if (wallXIndex == gameInnerSize.first + wallsSize.first) {
				++wallYIndex;
				wallXIndex = wallsSize.first;
			}
		}
	}
}

void Snake::makeCellFreeWithoutWallContainer() {
	_cellsFreeWithoutWalls.clear();
	set<cell_type> cellsFree;

	for (index_type i = wallsSize.first; i < wallsSize.first + gameInnerSize.first; ++i)
		for (index_type j = wallsSize.second; j < wallsSize.second + gameInnerSize.second; ++j) {
			cellsFree.insert(cell_type(i, j));
		}

	set_difference(cellsFree.begin(), cellsFree.end(), _walls.begin(), _walls.end(), inserter(_cellsFreeWithoutWalls, _cellsFreeWithoutWalls.end()));
}

Snake::State Snake::getState() {
	return _state;
}

void Snake::setState(State state) {
	_state = state;

	switch(_state){
	case READY:
		_title = titleStart;
		_titlePosition = _titleStartPosition;
		break;
	case GAME:
		_title = "";
		break;
	case END:
		_title = titleEnd;
		_titlePosition = _titleEndPosition;
		break;
	}
}

void Snake::setScore(long value) {
	_score = value;
	_scorePosition.mX = (_gameSize.first * _cellPixelSize - _font->StringWidth(to_string(_score))) / 2;
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

	if (cell != *(++_snake.rbegin())) {
		_direction = directionNew;
	}
}

void Snake::next() {
	cell_type cell = _snake.back();
	cell.first += _direction.first;
	cell.second += _direction.second;

	bool isEndByWall = _walls.find(cell) != _walls.end()
		|| cell.first < wallsSize.first
		|| cell.first > wallsSize.first + gameInnerSize.first - 1
		|| cell.second < wallsSize.second
		|| cell.second > wallsSize.second + gameInnerSize.second - 1;
	bool isEndBySnake = find(++_snake.begin(), _snake.end(), cell) != _snake.end();

	if (isEndByWall || isEndBySnake) {
		_cellEndCaused = make_shared<cell_type>(cell.first, cell.second);
		setState(END);

		if(isEndByWall) {
			_cellEndCausedColor = endCausedWallColor;
		}
		else if (isEndBySnake) {
			_cellEndCausedColor = endCausedSnakeColor;
		}
	}
	else {
		_snake.push_back(cell);

		if (_food && cell.first == _food->first && cell.second == _food->second) {
			_food = findCellFree();
			setScore(_score + scoreByFood);
		}
		else {
			_snake.pop_front();
		}
	}
}

void Snake::Update(float dt) {
	if (_state == GAME) {
		_stepTimer.first += dt;

		if (_stepTimer.first >= _stepTimer.second) {
			next();
			_stepTimer.first -= _stepTimer.second;
		}
	}
}

void Snake::Draw(Graphics* g) {
	g->Translate(_drawPosition.mX, _drawPosition.mY);
	g->SetColor(wallColor);

	for (auto wall : _wallsPoly) {
		g->PolyFill(&wall[0], wall.size());
	}
	
	for (auto wall : _walls) {
		g->FillRect(Rect(wall.first * _cellPixelSize, wall.second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	}

	auto snakeIt = _snake.rbegin();
	g->SetColor(_state == END ? snakeHeadEndColor : snakeHeadColor);
	g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	g->SetColor(snakeColor);
	++snakeIt;

	while (snakeIt != _snake.rend()) {
		g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
		++snakeIt;
	}

	if (_food) {
		g->SetColor(foodColor);
		g->FillRect(Rect(_food->first * _cellPixelSize, _food->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	}

	if (_cellEndCaused) {
		g->SetColor(_cellEndCausedColor);
		g->FillRect(Rect(_cellEndCaused->first * _cellPixelSize, _cellEndCaused->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	}

	g->SetColor(fontColor);
	g->SetFont(_font);
	g->DrawString(_title, _titlePosition.mX, _titlePosition.mY);

	if (_state != READY) {
		g->DrawString(to_string(_score), _scorePosition.mX, _scorePosition.mY);
	}
}