#include "stdafx.h"
#include "Board.h"
#include "SexyAppFramework/Graphics.h"

using namespace Sexy;
using namespace Game;

const Color backgroundColor = Color(32, 32, 32);

Board::Board(Snake::HardPtr snake, int syncRefreshRate)
	: _snake(snake)
	, _syncRefreshRate(syncRefreshRate)
{}

void Board::UpdateF(float theFrac) {
	Widget::UpdateF(theFrac);

	float dt = theFrac / _syncRefreshRate; // получаем приращение времени с последнего кадра
	_snake->Update(dt);
	MarkDirty();
}

void Board::Draw(Graphics* g) {
	g->SetColor(backgroundColor);
	g->FillRect(0, 0, mWidth, mHeight);
	_snake->Draw(g);	
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
			_snake->setState(Snake::GAME);
		}
	}
	else if(snakeState == Snake::END 
		&& !_snake->isGameOverTimer()) {
		// только когда закончили моргать надпись конца игры, то можем начать сначала
		_snake->reset();
	}
}