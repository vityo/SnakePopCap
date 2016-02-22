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

const Snake::cell_type gameInnerSize(16, 13); // внутренние размеры игрового поля (без стен)
const Snake::cell_type wallsSize(1, 1); // толщина стен
const uint8_t snakeFirstSize(3); // размер, с которым рождается змейка
const float stepPeriod = 0.3f; // шаг змейки происходит за это время в секундах
const Color wallColor = Color(200, 0, 0); // цвет стен
const Color snakeColor = Color(0, 150, 0); // цвет змейки
const Color snakeHeadColor = Color(0, 200, 0); // цвет головы змейки
const Color snakeHeadEndColor = Color(0, 100, 0); // цвет головы змейки, если ударилась
const Color endCausedWallColor = Color(100, 0, 0); // цвет стены, в которую врезались
const Color endCausedSnakeColor = Color(0, 50, 0); // цвет тела змейки, если в него ударились
const Color foodColor = Color(0, 0, 200); // цвет еды для змейки
const Color fontColor = Color(255, 255, 255); // цвет текста
const vector<Color> priseColors = { Color(200, 255, 0), Color(0, 255, 0), Color(0, 200, 255) }; // цвета призов
const vector<string> priseTitle = { "SCORE x3!", "SLICE!", "SPEED!" }; // тексты заголовков для призов
const long priseScoreMul = 3; // коэффициент приза очков
const long priseSliceCount = 5; // обрезаем змейку на столько ячеек после взятия приза
const float priseSpeedMul = 2.0f; // коэффициент приза времени
const float prisePeriod = 7.0f; // период исчезновения приза
const float gameOverPeriod = 3.0f; // период моргания текста после завершения игры
const string titleStart = "Press any arrow keys to start!"; // заголовок в начале игры
const string titleEnd = "GAME OVER. Press any key to start again!"; // заголовок в конце игры
const string fontId = "FONT_DEFAULT"; // шрифт
const long scoreByFood = 10; // очков за еду
const uint8_t foodCountBeforePrise = 1; // столько должны съесть еды, прежде чем появится приз
const float dtStepCoefficientDefault = 1.0f; // коэффициент времени по умолчанию
const long scoreCoefficientDefault = 1; // коэффициент получаемых очков по умолчанию


Snake::Snake(shared_ptr<Buffer> data, pair<int, int> screenSize)
	: _cellEndCausedColor(endCausedWallColor)
	, _gameOverTimer(0.f, gameOverPeriod)
{ 
	_font = GameApp::instance()->mResourceManager->GetFontThrow(fontId);
	load(data);
	makeCellFreeWithoutWallContainer();
	calcDrawSizes(screenSize);
	reset();
}

void Snake::reset() {
	_scoreCoefficient = scoreCoefficientDefault;
	_dtStepCoefficient = dtStepCoefficientDefault;
	_foodCountBeforePrise = 0;
	setScore(0);
	_cellEndCaused = nullptr;

	// выбираем место для старта змейки
	shared_ptr<cell_type> snakeStart = make_shared<cell_type>(wallsSize.first + static_cast<index_type>(gameInnerSize.first * 0.5f),
		wallsSize.second + static_cast<index_type>(gameInnerSize.second * 0.5f));
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
	_stepTimer = make_pair(0.f, stepPeriod);
	setDirection(RIGHT);
	setState(READY);
	_food = findCellFree();
	_prise = nullptr;
	_priseTimer = make_pair(prisePeriod, prisePeriod);
	_priseDraw = false;
	_gameOverTimer.first = 0.f;
	_titleDraw = true;
}

bool Snake::isGameOverTimer() {
	return _gameOverTimer.first < _gameOverTimer.second;
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

		// из множества пустых ячеек внутриигровой области вычитаем ячейки змейки и еды
		set_difference(cellsFreeWithoutWalls.begin(), cellsFreeWithoutWalls.end(), snakeAndFood.begin(), snakeAndFood.end(), inserter(cellsFree, cellsFree.end()));
	}

	// в cellsFree ячейки, из которых можем выбрать
	if (cellsFree.size()) {
		vector<cell_type>::size_type cellFreeIndex = utils::random(0, cellsFree.size() - 1);;
		auto cellIt = cellsFree.begin();
		advance(cellIt, cellFreeIndex);
		cellFree = make_shared<cell_type>(cellIt->first, cellIt->second);

		if (forPrise) {
			int priseIndex = utils::random(0, PRISE_COUNT - 1);
			_priseType = static_cast<Prise>(priseIndex);
			_priseColor = priseColors.at(priseIndex);
		}
	}

	return cellFree;
}

Point Snake::getTitlePosition(string title) {
	return Point((_gameSize.first * _cellPixelSize - _font->StringWidth(title)) / 2, static_cast<int>(_cellPixelSize - _fontHeight));
}

void Snake::calcDrawSizes(pair<int, int> screenSize) {
	// выбираем размеры для ячейки, все относительно
	float screenAspect = static_cast<float>(screenSize.first) / static_cast<float>(screenSize.second);
	_gameSize = make_pair(gameInnerSize.first + wallsSize.first * 2, gameInnerSize.second + wallsSize.second * 2);
	float gameAspect = static_cast<float>(_gameSize.first) / static_cast<float>(_gameSize.second);
	_cellPixelSize = screenAspect > gameAspect ? static_cast<pixel_type>(static_cast<float>(screenSize.second) / static_cast<float>(_gameSize.second))
			: static_cast<pixel_type>(static_cast<float>(screenSize.first) / static_cast<float>(_gameSize.first));
	_drawPosition = Point(static_cast<int>((screenSize.first - _cellPixelSize * _gameSize.first) * 0.5f),
		static_cast<int>((screenSize.second - _cellPixelSize * _gameSize.second) * 0.5f));

	// создаем набор точек из стен, окружающих внутриигровую область для быстрой полиотрисовки
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

	_fontHeight = _font->GetHeight();
	
	_titleStartPosition = getTitlePosition(titleStart);
	_titleEndPosition = getTitlePosition(titleEnd);
	_scorePosition.mY = static_cast<int>(_gameSize.second * _cellPixelSize - _fontHeight);

	for (auto titleString : priseTitle) {
		_priseTitlePositions.push_back(getTitlePosition(titleString));
	}
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

	// из множества ячеек внутриигровой области вычитаем ячейки стен внутриигровой области
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

void Snake::runPrise(Prise type) {
	_scoreCoefficient = scoreCoefficientDefault;
	_dtStepCoefficient = dtStepCoefficientDefault;

	switch (type) {
	case SCORE:
		_scoreCoefficient = priseScoreMul;
		break;
	case SLICE: 
	{
		int sliceCount = _snake.size() - max(1, _snake.size() - priseSliceCount);

		while (sliceCount--) {
			_snake.pop_front();
		}
	}
		break;
	case SPEED:
		_dtStepCoefficient = priseSpeedMul;
		break;
	}
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

	if (_snake.size() == 1 // если осталась только голова, то пусть двигается куда хочет
		|| cell != *(++_snake.rbegin())) {
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
		// во что-то ударились, завершаем игру
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
		// игра продолжается
		_snake.push_back(cell);

		if (_food && cell.first == _food->first && cell.second == _food->second) {
			// съели еду
			_food = findCellFree();
			setScore(_score + scoreByFood * _scoreCoefficient);
			++_foodCountBeforePrise;

			if (_foodCountBeforePrise == foodCountBeforePrise) {
				_foodCountBeforePrise = 0;
				_prise = findCellFree(true);
				_priseTimer.first = 0.f;
			}
		}
		else if (_prise && cell.first == _prise->first && cell.second == _prise->second) {
			// съели приз
			int priseIndex = static_cast<int>(_priseType);
			_title = priseTitle.at(priseIndex);
			_titlePosition = _priseTitlePositions.at(priseIndex);
			_prise = nullptr;
			_priseTimer.first = _priseTimer.second;
			runPrise(_priseType);
		}
		else {
			_snake.pop_front();
		}
	}
}

void Snake::Update(float dt) {
	// обрабатываем таймер шага змейки
	if (_state == GAME) {
		_stepTimer.first += dt * _dtStepCoefficient;;

		if (_stepTimer.first >= _stepTimer.second) {
			next();
			_stepTimer.first -= _stepTimer.second;
		}
	}

	// остальные таймеры ходят даже после конца игры
	if (_priseTimer.first < _priseTimer.second) {
		_priseTimer.first += dt;

		if (_priseTimer.first >= _priseTimer.second) {
			_priseTimer.first = _priseTimer.second;
			_priseDraw = false;
			_prise = nullptr;
		}
		else {
			_priseDraw = utils::ease(_priseTimer.first / _priseTimer.second);
		}
	}

	if (_state == END && _gameOverTimer.first < _gameOverTimer.second) {
		_gameOverTimer.first += dt;

		if (_gameOverTimer.first >= _gameOverTimer.second) {
			_gameOverTimer.first = _gameOverTimer.second;
			_titleDraw = true;
		}
		else {
			_titleDraw = utils::ease((_gameOverTimer.second - _gameOverTimer.first) / _gameOverTimer.second);
		}
	}
}

void Snake::Draw(Graphics* g) {
	g->Translate(_drawPosition.mX, _drawPosition.mY);
	// стены
	g->SetColor(wallColor);

	for (auto wall : _wallsPoly) {
		g->PolyFill(&wall[0], wall.size());
	}
	
	for (auto wall : _walls) {
		g->FillRect(Rect(wall.first * _cellPixelSize, wall.second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	}

	// змейка
	auto snakeIt = _snake.rbegin();
	g->SetColor(_state == END ? snakeHeadEndColor : snakeHeadColor);
	g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	g->SetColor(snakeColor);
	++snakeIt;

	while (snakeIt != _snake.rend()) {
		g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
		++snakeIt;
	}

	// еда
	if (_food) {
		g->SetColor(foodColor);
		g->FillRect(Rect(_food->first * _cellPixelSize, _food->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	}

	// приз
	if (_prise && _priseTimer.first < _priseTimer.second
		&& _priseDraw) {
		g->SetColor(_priseColor);
		g->FillRect(Rect(_prise->first * _cellPixelSize, _prise->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	}

	// ячейка, по причине смерти
	if (_cellEndCaused) {
		g->SetColor(_cellEndCausedColor);
		g->FillRect(Rect(_cellEndCaused->first * _cellPixelSize, _cellEndCaused->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	}

	// заголовок
	g->SetColor(fontColor);
	g->SetFont(_font);

	if (_titleDraw) {
		g->DrawString(_title, _titlePosition.mX, _titlePosition.mY);
	}

	// очки
	if (_state != READY) {
		g->DrawString(to_string(_score), _scorePosition.mX, _scorePosition.mY);
	}
}