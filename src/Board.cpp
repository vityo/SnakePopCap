#include "stdafx.h"
#include "Board.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/ResourceManager.h"
#include "GameApp.h"
#include "Utils.h"

using namespace Sexy;
using namespace Game;

const string fontId = "FONT_DEFAULT"; // �����
const Color fontColor = Color(255, 255, 255); // ���� ������
const string titleStart = "Press any arrow keys to start!"; // ��������� � ������ ����
const string titleEnd = "GAME OVER. Press any key to start again!"; // ��������� � ����� ����
const string titlePriseScore = "SCORE x3!"; // ����� ����� �����
const string titlePriseSlice = "SLICE!"; // ����� ����� �������
const string titlePriseSpeed = "SPEED!"; // ����� ����� ��������
const Color priseScoreColor = Color(200, 255, 0); // ���� ����� �����
const Color priseSliceColor = Color(0, 255, 0); // ���� ����� �������
const Color priseSpeedColor = Color(0, 200, 255); // ���� ����� ��������
const Color backgroundColor = Color(32, 32, 32); // ���� ����
const Color wallColor = Color(200, 0, 0); // ���� ����
const Color foodColor = Color(0, 0, 200); // ���� ��� ��� ������
const Color snakeHeadColor = Color(0, 200, 0); // ���� ������ ������
const Color snakeColor = Color(0, 150, 0); // ���� ������
const Color snakeHeadEndColor = Color(0, 100, 0); // ���� ������ ������, ���� ���������
const Color endCausedWallColor = Color(100, 0, 0); // ���� �����, � ������� ���������
const float gameOverPeriod = 3.0f; // ������ �������� ������ ����� ���������� ����
const float stepPeriod = 0.3f; // ��� ������ ���������� �� ��� ����� � ��������
const float dtStepCoefficientDefault = 1.0f; // ����������� ������� �� ���������
const float priseSpeedMul = 2.0f; // ����������� ����� �������
const float prisePeriod = 7.0f; // ������ ������������ �����

Point Board::Title::_positionCenter;
Font* Board::Title::_font = nullptr;

void Board::Title::Init(Font* font, Point positionCenter) {
	_font = font;
	_positionCenter = positionCenter;
}

Board::Title::Title() 
	: _caption("")
	, _position(Point())
{}

Board::Title::Title(string caption) { 
	setCaption(caption); 
}

void Board::Title::setCaption(string value) {
	_caption = value;
	_position = getTitlePosition(_caption);
}

string Board::Title::getCaption() { 
	return _caption; 
}

Point Board::Title::getPosition() {
	return _position; 
}

void Board::Title::setPosition(Point value) {
	_position = value;
}

Font* Board::Title::getFont() {
	return _font;
}

Point Board::Title::getTitlePosition(string _caption) {
	Assert(_font);
	return Point((_positionCenter.mX - _font->StringWidth(_caption)) / 2, _positionCenter.mY);
}

Board::Board(Snake::HardPtr snake, int syncRefreshRate, pair<int, int> screenSize)
	: _snake(snake)
	, _syncRefreshRate(syncRefreshRate)
	, _cellEndCausedColor(endCausedWallColor)
	, _gameOverTimer(0.f, gameOverPeriod)
	, _stepTimer(0.f, stepPeriod)
	, _priseDraw(false)
	, _titleDraw(false)
{
	// �������� ������� ��� ������, ��� ������������
	float screenAspect = static_cast<float>(screenSize.first) / static_cast<float>(screenSize.second);
	pair<Snake::index_type, Snake::index_type> gameSize = _snake->getGameSize();
	float gameAspect = static_cast<float>(gameSize.first) / static_cast<float>(gameSize.second);
	_cellPixelSize = screenAspect > gameAspect ? static_cast<int>(static_cast<float>(screenSize.second) / static_cast<float>(gameSize.second))
		: static_cast<int>(static_cast<float>(screenSize.first) / static_cast<float>(gameSize.first));
	_drawPosition = Point(static_cast<int>((screenSize.first - _cellPixelSize * gameSize.first) * 0.5f),
		static_cast<int>((screenSize.second - _cellPixelSize * gameSize.second) * 0.5f));

	// ������� ����� ����� �� ����, ���������� ������������� ������� ��� ������� �������������
	Snake::cell_type wallsSize = _snake->getWallsSize();
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

	// ��������� ��� ������ � ���������
	int gameWidth = gameSize.first * _cellPixelSize;
	Font* font = GameApp::instance()->mResourceManager->GetFontThrow(fontId);
	int fontHeight = font->GetHeight();
	Title::Init(font, Point(gameWidth, static_cast<int>(_cellPixelSize - fontHeight)));
	_titleScore = Title();
	Point titleScorePosition = _titleScore.getPosition();
	titleScorePosition.mY = static_cast<int>(gameSize.second * _cellPixelSize - fontHeight);
	_titleScore.setPosition(titleScorePosition);
	_priseParameters.insert(make_pair(Snake::SCORE,
		PriseParameters(Title(titlePriseScore), priseScoreColor)));
	_priseParameters.insert(make_pair(Snake::SLICE,
		PriseParameters(Title(titlePriseSlice), priseSliceColor)));
	_priseParameters.insert(make_pair(Snake::SPEED,
		PriseParameters(Title(titlePriseSpeed), priseSpeedColor)));
	_stateParameters.insert(make_pair(Snake::READY, Title(titleStart)));
	_stateParameters.insert(make_pair(Snake::END, Title(titleEnd)));
}

void Board::UpdateF(float theFrac) {
	Widget::UpdateF(theFrac);

	float dt = theFrac / _syncRefreshRate; // �������� ���������� ������� � ���������� �����

	// ������������ ������ ���� ������
	if (_snake->getState() == Snake::GAME) {
		_stepTimer.first += dt * _dtStepCoefficient;;

		if (_stepTimer.first >= _stepTimer.second) {
			if (_snake->next()) {
				_stepTimer.first -= _stepTimer.second;

				if (_snake->getPriseTypeActive() == Snake::SPEED) {
					_dtStepCoefficient = priseSpeedMul;
				}
				else {
					_dtStepCoefficient = dtStepCoefficientDefault;
				}

				_titleScore.setCaption(to_string(_snake->getScore()));
			}
		}
	}
	else if(_snake->getState() == Snake::READY 
		|| _snake->getState() == Snake::END) {
		_title = _stateParameters.at(_snake->getState());
	}

	auto priseParametersIt = _priseParameters.find(_snake->getPriseTypeActive());

	if (priseParametersIt != _priseParameters.end()) {
		_title = priseParametersIt->second.title;

		if (_priseTimer.first < _priseTimer.second) {
			_priseTimer.first += dt;

			if (_priseTimer.first >= _priseTimer.second) {
				_priseTimer.first = _priseTimer.second;
				_priseDraw = false;
				_snake->getPrise()->expired = true;
			}
			else {
				_priseDraw = utils::Math::ease(_priseTimer.first / _priseTimer.second);
			}
		}
	}

	MarkDirty();
}

void Board::Draw(Graphics* g) {
	g->SetColor(backgroundColor);
	g->FillRect(0, 0, mWidth, mHeight);

	g->Translate(_drawPosition.mX, _drawPosition.mY);
	// �����
	g->SetColor(wallColor);

	for (auto wall : _wallsPoly) {
		g->PolyFill(&wall[0], wall.size());
	}

	for (auto wall : _snake->getWalls()) {
		g->FillRect(Rect(wall.first * _cellPixelSize, wall.second * _cellPixelSize, 
			_cellPixelSize, _cellPixelSize));
	}

	// ������
	auto snakeIt = _snake->getSnake().rbegin();
	g->SetColor(_snake->getState() == Snake::END ? snakeHeadEndColor : snakeHeadColor);
	g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize, 
		_cellPixelSize, _cellPixelSize));
	g->SetColor(snakeColor);
	++snakeIt;

	while (snakeIt != _snake->getSnake().rend()) {
		g->FillRect(Rect(snakeIt->first * _cellPixelSize, snakeIt->second * _cellPixelSize, 
			_cellPixelSize, _cellPixelSize));
		++snakeIt;
	}

	// ���
	if (_snake->getFood()) {
		g->SetColor(foodColor);
		g->FillRect(Rect(_snake->getFood()->first * _cellPixelSize, _snake->getFood()->second * _cellPixelSize, 
			_cellPixelSize, _cellPixelSize));
	}

	// ����
	if (_snake->getPrise() &&  _priseDraw) {
		g->SetColor(_priseParameters.at(_snake->getPriseTypeActive()).color);
		g->FillRect(Rect(_snake->getPrise()->position.first * _cellPixelSize, _snake->getPrise()->position.second * _cellPixelSize,
			_cellPixelSize, _cellPixelSize));
	}

	// ������, �� ������� ������
	if (_snake->get�ellEndCaused()) {
		g->SetColor(_cellEndCausedColor);
		g->FillRect(Rect(_snake->get�ellEndCaused()->first * _cellPixelSize, 
			_snake->get�ellEndCaused()->second * _cellPixelSize, _cellPixelSize, _cellPixelSize));
	}

	// ���������
	g->SetColor(fontColor);
	g->SetFont(Title::getFont());

	if (_titleDraw) {
		g->DrawString(_title.getCaption(), _title.getPosition().mX, _title.getPosition().mY);
	}

	// ����
	if (_snake->getState() != Snake::READY) {
		g->DrawString(_titleScore.getCaption(), _titleScore.getPosition().mX, _titleScore.getPosition().mY);
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
		}
	}
	else if(snakeState == Snake::END 
		&& _gameOverTimer.first >= _gameOverTimer.second) {
		// ������ ����� ��������� ������� ������� ����� ����, �� ����� ������ �������
		_snake->reset();
		_gameOverTimer.first = 0.f;
		_titleDraw = false;
		_stepTimer.first = 0.f;
		_priseTimer.first = _priseTimer.second;
		_priseDraw = false;
		_dtStepCoefficient = dtStepCoefficientDefault;
	}
}