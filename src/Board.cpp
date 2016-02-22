#include "stdafx.h"
#include "Board.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/ResourceManager.h"
#include "GameApp.h"
#include "Utils.h"
#include "GameInfo.h"

using namespace Sexy;
using namespace Game;

Font* Board::Title::_font = nullptr;

void Board::Title::Init(Font* font) {
	_font = font;
	Assert(_font);
}

Board::Title::Title() 
	: _caption("")
	, _positionCenter(Point())
	, _shiftX(0)
{}

Board::Title::Title(string caption, Point positionCenter) {
	setCaption(caption); 
	_positionCenter = positionCenter;
}

void Board::Title::setCaption(string value) {
	_caption = value;
	calcTitleShiftX(_caption);
}

string Board::Title::getCaption() { 
	return _caption; 
}

Point Board::Title::getPositionDraw() {
	return Point(_positionCenter.mX + _shiftX, _positionCenter.mY);
}

void Board::Title::setPositionCenter(Point value) {
	_positionCenter = value;
}

Font* Board::Title::getFont() {
	return _font;
}

void Board::Title::calcTitleShiftX(string _caption) {
	_shiftX = -_font->StringWidth(_caption) / 2;
}

Board::Board(int syncRefreshRate, pair<int, int> screenSize, shared_ptr<Buffer> data)
	: _syncRefreshRate(syncRefreshRate)
	, _cellEndCausedColor(endCausedWallColor)
{
	_snake = make_shared<Snake>(make_pair(1, 1), load(data));
	Assert(_snake);

	// выбираем размеры для ячейки, все относительно
	float screenAspect = static_cast<float>(screenSize.first) / static_cast<float>(screenSize.second);
	pair<Snake::index_type, Snake::index_type> gameSize = _snake->getGameSize();
	float gameAspect = static_cast<float>(gameSize.first) / static_cast<float>(gameSize.second);
	_cellPixelSize = screenAspect > gameAspect ? static_cast<int>(static_cast<float>(screenSize.second) / static_cast<float>(gameSize.second))
		: static_cast<int>(static_cast<float>(screenSize.first) / static_cast<float>(gameSize.first));
	_drawPosition = Point(static_cast<int>((screenSize.first - _cellPixelSize * gameSize.first) * 0.5f),
		static_cast<int>((screenSize.second - _cellPixelSize * gameSize.second) * 0.5f));

	// создаем набор точек из стен, окружающих внутриигровую область для быстрой полиотрисовки
	_wallsPoly = { { Point(0, 0),
		Point(0, gameSize.second * _cellPixelSize),
		Point(gameSize.first * _cellPixelSize, gameSize.second * _cellPixelSize),
		Point(gameSize.first * _cellPixelSize, (gameSize.second - wallsSize.second) * _cellPixelSize),
		Point(wallsSize.first * _cellPixelSize, (gameSize.second - wallsSize.second) * _cellPixelSize),
		Point(wallsSize.first * _cellPixelSize, 0) },
		{ Point(wallsSize.first * _cellPixelSize, 0),
		Point(wallsSize.first * _cellPixelSize, wallsSize.second * _cellPixelSize),
		Point((gameSize.first - wallsSize.first) * _cellPixelSize, wallsSize.second * _cellPixelSize),
		Point((gameSize.first - wallsSize.first) * _cellPixelSize, (gameSize.second - wallsSize.second) * _cellPixelSize),
		Point(gameSize.first * _cellPixelSize, (gameSize.second - wallsSize.second) * _cellPixelSize),
		Point(gameSize.first * _cellPixelSize, 0) } };

	// параметры для призов и состояний
	int gameWidth = gameSize.first * _cellPixelSize;
	Font* font = GameApp::instance()->mResourceManager->GetFontThrow(fontId);
	int fontHeight = font->GetHeight();
	Title::Init(font);
	Point titleCenterPosition = Point(gameWidth / 2, static_cast<int>(_cellPixelSize - fontHeight));
	_titleScore = Title("", Point(titleCenterPosition.mX, static_cast<int>(gameSize.second * _cellPixelSize - fontHeight)));
	_priseParameters.insert(make_pair(Snake::SCORE,
		PriseParameters(Title(titlePriseScore, titleCenterPosition), priseScoreColor)));
	_priseParameters.insert(make_pair(Snake::SLICE,
		PriseParameters(Title(titlePriseSlice, titleCenterPosition), priseSliceColor)));
	_priseParameters.insert(make_pair(Snake::SPEED,
		PriseParameters(Title(titlePriseSpeed, titleCenterPosition), priseSpeedColor)));
	_stateParameters.insert(make_pair(Snake::READY, Title(titleStart, titleCenterPosition)));
	_stateParameters.insert(make_pair(Snake::END, Title(titleEnd, titleCenterPosition)));

	reset();
}

set<Game::Snake::cell_type> Board::load(shared_ptr<Buffer> data) {
	// читаем файл карты со стенами. по максимуму используем функции PopCap, которые работают, ничего в нем не меняя. Например, ReadBuffer, ReadString - уже падают
	set<Snake::cell_type> walls;
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
				walls.insert(Snake::cell_type(wallXIndex, wallYIndex));
			}

			++wallXIndex;

			if (wallXIndex == gameInnerSize.first + wallsSize.first) {
				++wallYIndex;
				wallXIndex = wallsSize.first;
			}
		}
	}

	return walls;
}

void Board::UpdateF(float theFrac) {
	Widget::UpdateF(theFrac);

	float dt = theFrac / _syncRefreshRate; // получаем приращение времени с последнего кадра

	// обрабатываем таймер шага змейки
	if (_snake->getState() == Snake::GAME) {
		_stepTimer.first += dt * _dtStepCoefficient;;

		if (_stepTimer.first >= _stepTimer.second) {
			Snake::EventType eventType = _snake->next();

			if (eventType != Snake::EVENT_WALL 
				&& eventType != Snake::EVENT_SNAKE) {
				_stepTimer.first -= _stepTimer.second;

				if (eventType == Snake::EVENT_PRISE_EAT) {
					if (_snake->getPriseTypeActive() == Snake::SPEED) {
						_dtStepCoefficient = priseSpeedMul;
					}
					else {
						_dtStepCoefficient = dtStepCoefficientDefault;
					}

					auto priseParametersIt = _priseParameters.find(_snake->getPriseTypeActive());

					if (priseParametersIt != _priseParameters.end()) {
						_title = priseParametersIt->second.title;
						_titleTimer.first = 0.f;
						_titleTimer.second = priseActivePeriod; 
					}
				}
				else if (eventType == Snake::EVENT_PRISE_CREATE) {
					_priseDisappearTimer.first = 0.f;
				}

				_titleScore.setCaption(to_string(_snake->getScore()));
			}
			else {
				_titleTimer.first = 0.f;
				_titleTimer.second = gameOverPeriod;
				_title = _stateParameters.at(_snake->getState());
			}
		}
	}
	
	// время действия призов или конец игры, моргает заголовок
	if (_titleTimer.first < _titleTimer.second) {
		_titleTimer.first += dt;

		if (_titleTimer.first >= _titleTimer.second) {
			_titleTimer.first = _titleTimer.second;

			if (_snake->getState() == Snake::END) {
				_titleDraw = true;
			}
			else {
				_titleDraw = false;
				_snake->priseActiveEnd();
				_dtStepCoefficient = dtStepCoefficientDefault;
			}
		}
		else {
			if (_snake->getState() == Snake::END) {
				// моргаем конец игры
				_titleDraw = utils::Math::ease((_titleTimer.second - _titleTimer.first) / _titleTimer.second);
			}
			else {
				// моргают заголовки призов
				_titleDraw = utils::Math::ease(_titleTimer.first / _titleTimer.second);
			}
		}
	}

	// моргают призы
	if(_snake->getPrise()) {
		if (_priseDisappearTimer.first < _priseDisappearTimer.second) {
			_priseDisappearTimer.first += dt;

			if (_priseDisappearTimer.first >= _priseDisappearTimer.second) {
				_priseDisappearTimer.first = _priseDisappearTimer.second;
				_priseDraw = false;
				_snake->priseDisappear();
			}
			else {
				_priseDraw = utils::Math::ease(_priseDisappearTimer.first / _priseDisappearTimer.second);
			}
		}
	}

	MarkDirty();
}

void Board::Draw(Graphics* g) {
	g->SetColor(backgroundColor);
	g->FillRect(0, 0, mWidth, mHeight);

	g->Translate(_drawPosition.mX, _drawPosition.mY);
	// стены
	g->SetColor(wallColor);

	for (auto wall : _wallsPoly) {
		g->PolyFill(&wall[0], wall.size());
	}

	if (_snake) {
		for (auto wall : _snake->getWalls()) {
			g->FillRect(Rect(wall.first * _cellPixelSize, wall.second * _cellPixelSize,
				_cellPixelSize, _cellPixelSize));
		}

		// змейка
		if (!_snake->getSnake().empty()) {
			g->SetColor(_snake->getState() == Snake::END ? snakeHeadEndColor : snakeHeadColor);
			auto snakeIt = _snake->getSnake().rbegin();
			g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize,
				_cellPixelSize, _cellPixelSize));
			g->SetColor(snakeColor);
			++snakeIt;

			while (snakeIt != _snake->getSnake().rend()) {
				g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize,
					_cellPixelSize, _cellPixelSize));
				++snakeIt;
			}
		}

		// еда
		if (_snake->getFood()) {
			g->SetColor(foodColor);
			g->FillRect(Rect(_snake->getFood()->first * _cellPixelSize, _snake->getFood()->second * _cellPixelSize,
				_cellPixelSize, _cellPixelSize));
		}

		// приз
		if (_snake->getPrise() && _priseDraw) {
			g->SetColor(_priseParameters.at(_snake->getPrise()->type).color);
			g->FillRect(Rect(_snake->getPrise()->position.first * _cellPixelSize, _snake->getPrise()->position.second * _cellPixelSize,
				_cellPixelSize, _cellPixelSize));
		}

		// ячейка, по причине смерти
		if (_snake->getСellEndCaused()) {
			g->SetColor(_cellEndCausedColor);
			g->FillRect(Rect(_snake->getСellEndCaused()->first * _cellPixelSize,
				_snake->getСellEndCaused()->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
		}

		
	}

	// тексты
	g->SetColor(fontColor);
	g->SetFont(Title::getFont());

	// очки
	if (_snake && _snake->getState() != Snake::READY) {
		Point titlePosition = _titleScore.getPositionDraw();
		g->DrawString(_titleScore.getCaption(), titlePosition.mX, titlePosition.mY);
	}

	// заголовок
	if (_titleDraw) {
		Point titlePosition = _title.getPositionDraw();
		g->DrawString(_title.getCaption(), titlePosition.mX, titlePosition.mY);
	}
}

void Board::KeyDown(KeyCode theKey) {
	Snake::State snakeState = _snake->getState();

	if (snakeState == Snake::READY
		&& !(theKey == KEYCODE_LEFT || theKey == KEYCODE_RIGHT
			|| theKey == KEYCODE_UP || theKey == KEYCODE_DOWN)) {
		return;
	}

	if (snakeState == Snake::READY ||
		snakeState == Snake::GAME) {
		switch (theKey) {
		case KEYCODE_LEFT:
			_snake->setDirection(Snake::LEFT);
			break;
		case KEYCODE_RIGHT:
			_snake->setDirection(Snake::RIGHT);
			break;
		case KEYCODE_UP:
			_snake->setDirection(Snake::UP);
			break;
		case KEYCODE_DOWN:
			_snake->setDirection(Snake::DOWN);
			break;
		}

		if (snakeState == Snake::READY) {
			_snake->start();
			_titleDraw = false;
		}
	}
	else if(snakeState == Snake::END 
		&& _titleTimer.first >= _titleTimer.second) {
		// только когда закончили моргать надпись конца игры, то можем начать сначала
		_snake->reset();
		reset();
	}
}

void Board::reset() {
	_stepTimer = make_pair(0.f, stepPeriod);
	_priseDisappearTimer = make_pair(priseDisappearPeriod, priseDisappearPeriod);
	_title = _stateParameters.at(_snake->getState());
	_titleDraw = true;
	_priseDraw = false;
	_dtStepCoefficient = dtStepCoefficientDefault;
}