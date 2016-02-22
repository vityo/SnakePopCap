#pragma once
#include "SexyAppFramework/Widget.h"
#include "Snake.h"

namespace Sexy {
	class Graphics;
	class GameApp;

	// виджет со змейкой
	class Board : public Widget {
	public:
		typedef shared_ptr<Board> HardPtr; // сильный указатель

		Board(Game::Snake::HardPtr snake, int syncRefreshRate); // конструктор, змейка и частота обновления
		virtual void Draw(Graphics* g); // отрисовка
		virtual void UpdateF(float theFrac); // обновление
		virtual void KeyDown(KeyCode theKey); // нажатия
	private:
		Game::Snake::HardPtr _snake; // змейка
		int _syncRefreshRate; // частота обновления
	};
}