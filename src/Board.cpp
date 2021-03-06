#include "stdafx.h"
#include "Board.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/Point.h"
#include "GameApp.h"
#include "Utils.h"
#include "GameInfo.h"

using namespace Sexy;
using namespace Game;

Font* Board::Title::_font = nullptr;
Color Board::Title::_color;

void Board::Title::Init(Font* font, Color color) {
	_font = font;
	Assert(_font);
	_color = color;
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

Color Board::Title::getColor() {
	return _color;
}

void Board::Title::calcTitleShiftX(string _caption) {
	_shiftX = -_font->StringWidth(_caption) / 2;
}

Board::Board(int syncRefreshRate, pair<int, int> screenSize, shared_ptr<Buffer> data)
	: _syncRefreshRate(syncRefreshRate)
	, _cellEndCausedColor(gameInfo.endCausedWallColor)
{
	_snake = make_shared<Snake>(gameInfo.wallsSize, load(data));
	Assert(_snake);

	// �������� ������� ��� ������, ��� ������������
	float screenAspect = static_cast<float>(screenSize.first) / static_cast<float>(screenSize.second);
	pair<Snake::index_type, Snake::index_type> gameSize = _snake->getGameSize();
	float gameAspect = static_cast<float>(gameSize.first) / static_cast<float>(gameSize.second);
	_cellPixelSize = screenAspect > gameAspect ? static_cast<int>(static_cast<float>(screenSize.second) / static_cast<float>(gameSize.second))
		: static_cast<int>(static_cast<float>(screenSize.first) / static_cast<float>(gameSize.first));
	_drawPosition = Point(static_cast<int>((screenSize.first - _cellPixelSize * gameSize.first) * 0.5f),
		static_cast<int>((screenSize.second - _cellPixelSize * gameSize.second) * 0.5f));

	// ������� ����� ����� �� ����, ���������� ������������� ������� ��� ������� �������������
	_wallsPoly = { { Point(0, 0),
		Point(0, gameSize.second * _cellPixelSize),
		Point(gameSize.first * _cellPixelSize, gameSize.second * _cellPixelSize),
		Point(gameSize.first * _cellPixelSize, (gameSize.second - gameInfo.wallsSize.second) * _cellPixelSize),
		Point(gameInfo.wallsSize.first * _cellPixelSize, (gameSize.second - gameInfo.wallsSize.second) * _cellPixelSize),
		Point(gameInfo.wallsSize.first * _cellPixelSize, 0) },
		{ Point(gameInfo.wallsSize.first * _cellPixelSize, 0),
		Point(gameInfo.wallsSize.first * _cellPixelSize, gameInfo.wallsSize.second * _cellPixelSize),
		Point((gameSize.first - gameInfo.wallsSize.first) * _cellPixelSize, gameInfo.wallsSize.second * _cellPixelSize),
		Point((gameSize.first - gameInfo.wallsSize.first) * _cellPixelSize, (gameSize.second - gameInfo.wallsSize.second) * _cellPixelSize),
		Point(gameSize.first * _cellPixelSize, (gameSize.second - gameInfo.wallsSize.second) * _cellPixelSize),
		Point(gameSize.first * _cellPixelSize, 0) } };

	// ��������� ��� ������ � ���������
	int gameWidth = gameSize.first * _cellPixelSize;
	int fontHeight = gameInfo.font->GetHeight();
	Title::Init(gameInfo.font, gameInfo.fontColor);
	Point titleCenterPosition = Point(gameWidth / 2, static_cast<int>(_cellPixelSize - fontHeight));
	_titleScore = Title("", Point(titleCenterPosition.mX, static_cast<int>(gameSize.second * _cellPixelSize - fontHeight)));
	_priseParameters.insert(make_pair(Snake::SCORE,
		PriseParameters(Title(gameInfo.titlePriseScore, titleCenterPosition), gameInfo.priseScoreColor)));
	_priseParameters.insert(make_pair(Snake::SLICE,
		PriseParameters(Title(gameInfo.titlePriseSlice, titleCenterPosition), gameInfo.priseSliceColor)));
	_priseParameters.insert(make_pair(Snake::SPEED,
		PriseParameters(Title(gameInfo.titlePriseSpeed, titleCenterPosition), gameInfo.priseSpeedColor)));
	_stateParameters.insert(make_pair(Snake::READY, Title(gameInfo.titleStart, titleCenterPosition)));
	_stateParameters.insert(make_pair(Snake::END, Title(gameInfo.titleEnd, titleCenterPosition)));

	reset();
}

set<Game::Snake::cell_type> Board::load(shared_ptr<Buffer> data) {
	// ������ ���� ����� �� �������. �� ��������� ���������� ������� PopCap, ������� ��������, ������ � ��� �� �����. ��������, ReadBuffer, ReadString - ��� ������
	set<Snake::cell_type> walls;
	ByteVector byteVector;
	const uint8_t symbols = 2; // ������� ������ � ������� �������
	int dataLength = (gameInfo.gameInnerSize.first + symbols) * gameInfo.gameInnerSize.second - symbols;
	int dataLengthActual = data->GetDataLen();
	string message = "Invalid card file. Card file should be " + to_string(gameInfo.gameInnerSize.first)
		+ "x" + to_string(gameInfo.gameInnerSize.second) + " size and contain non-whitespace characters in place of walls";
	Assert2(dataLength == dataLengthActual, message.c_str());
	byteVector.assign(dataLength, (uchar)0);
	data->ReadBytes(&byteVector[0], dataLengthActual);

	uint8_t wallXIndex = gameInfo.wallsSize.first;
	uint8_t wallYIndex = gameInfo.wallsSize.second;

	for (uchar ch : byteVector) {
		if (ch != '\n' && ch != '\r') {
			if (ch != ' ') {
				walls.insert(Snake::cell_type(wallXIndex, wallYIndex));
			}

			++wallXIndex;

			if (wallXIndex == gameInfo.gameInnerSize.first + gameInfo.wallsSize.first) {
				++wallYIndex;
				wallXIndex = gameInfo.wallsSize.first;
			}
		}
	}

	return walls;
}

void Board::UpdateF(float theFrac) {
	Widget::UpdateF(theFrac);

	float dt = theFrac / _syncRefreshRate; // �������� ���������� ������� � ���������� �����

	// ������������ ������ ���� ������
	if (_snake->getState() == Snake::GAME) {
		_stepTimer.first += dt * _dtStepCoefficient;;

		if (_stepTimer.first >= _stepTimer.second) {
			Snake::EventType eventType = _snake->next();

			if (eventType != Snake::EVENT_WALL 
				&& eventType != Snake::EVENT_SNAKE) {
				_stepTimer.first -= _stepTimer.second;

				if (eventType == Snake::EVENT_PRISE_EAT) {
					if (_snake->getPriseTypeActive() == Snake::SPEED) {
						_dtStepCoefficient = gameInfo.priseSpeedMul;
					}
					else {
						_dtStepCoefficient = gameInfo.dtStepCoefficientDefault;
					}

					auto priseParametersIt = _priseParameters.find(_snake->getPriseTypeActive());

					if (priseParametersIt != _priseParameters.end()) {
						_title = priseParametersIt->second.title;
						_titleTimer.first = 0.f;
						_titleTimer.second = gameInfo.priseActivePeriod;
					}
				}
				else if (eventType == Snake::EVENT_PRISE_CREATE) {
					_priseDisappearTimer.first = 0.f;
				}

				_titleScore.setCaption(to_string(_snake->getScore()));
			}
			else {
				_titleTimer.first = 0.f;
				_titleTimer.second = gameInfo.gameOverPeriod;
				_title = _stateParameters.at(_snake->getState());

				_cellEndCausedColor = eventType == Snake::EVENT_WALL ? gameInfo.endCausedWallColor : gameInfo.endCausedSnakeColor;
			}
		}
	}
	
	// ����� �������� ������ ��� ����� ����, ������� ���������
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
				_dtStepCoefficient = gameInfo.dtStepCoefficientDefault;
			}
		}
		else {
			if (_snake->getState() == Snake::END) {
				// ������� ����� ����
				_titleDraw = utils::Math::ease((_titleTimer.second - _titleTimer.first) / _titleTimer.second);
			}
			else {
				// ������� ��������� ������
				_titleDraw = utils::Math::ease(_titleTimer.first / _titleTimer.second);
			}
		}
	}

	// ������� �����
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
	g->SetColor(gameInfo.backgroundColor);
	g->FillRect(0, 0, mWidth, mHeight);

	g->Translate(_drawPosition.mX, _drawPosition.mY);
	// �����
	g->SetColor(gameInfo.wallColor);

	for (auto wall : _wallsPoly) {
		g->PolyFill(&wall[0], wall.size());
	}

	if (_snake) {
		for (auto wall : _snake->getWalls()) {
			g->FillRect(Rect(wall.first * _cellPixelSize, wall.second * _cellPixelSize,
				_cellPixelSize, _cellPixelSize));
		}

		// ������
		if (!_snake->getSnake().empty()) {
			g->SetColor(_snake->getState() == Snake::END ? gameInfo.snakeHeadEndColor : gameInfo.snakeHeadColor);
			auto snakeIt = _snake->getSnake().rbegin();
			g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize,
				_cellPixelSize, _cellPixelSize));
			g->SetColor(gameInfo.snakeColor);
			++snakeIt;

			while (snakeIt != _snake->getSnake().rend()) {
				g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize,
					_cellPixelSize, _cellPixelSize));
				++snakeIt;
			}
		}

		// ���
		if (_snake->getFood()) {
			g->SetColor(gameInfo.foodColor);
			g->FillRect(Rect(_snake->getFood()->first * _cellPixelSize, _snake->getFood()->second * _cellPixelSize,
				_cellPixelSize, _cellPixelSize));
		}

		// ����
		if (_snake->getPrise() && _priseDraw) {
			g->SetColor(_priseParameters.at(_snake->getPrise()->type).color);
			g->FillRect(Rect(_snake->getPrise()->position.first * _cellPixelSize, _snake->getPrise()->position.second * _cellPixelSize,
				_cellPixelSize, _cellPixelSize));
		}

		// ������, �� ������� ������
		if (_snake->get�ellEndCaused()) {
			g->SetColor(_cellEndCausedColor);
			g->FillRect(Rect(_snake->get�ellEndCaused()->first * _cellPixelSize,
				_snake->get�ellEndCaused()->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
		}
	}

	// ������
	g->SetColor(Title::getColor());
	g->SetFont(Title::getFont());

	// ����
	if (_snake && _snake->getState() != Snake::READY) {
		Point titlePosition = _titleScore.getPositionDraw();
		g->DrawString(_titleScore.getCaption(), titlePosition.mX, titlePosition.mY);
	}

	// ���������
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
		// ������ ����� ��������� ������� ������� ����� ����, �� ����� ������ �������
		_snake->reset();
		reset();
	}
}

void Board::reset() {
	_stepTimer = make_pair(0.f, gameInfo.stepPeriod);
	_priseDisappearTimer = make_pair(gameInfo.priseDisappearPeriod, gameInfo.priseDisappearPeriod);
	_title = _stateParameters.at(_snake->getState());
	_titleDraw = true;
	_priseDraw = false;
	_dtStepCoefficient = gameInfo.dtStepCoefficientDefault;
	_titleScore.setCaption(to_string(_snake->getScore()));
}