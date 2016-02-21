#pragma once
#include "SexyAppFramework/Widget.h"
#include "Snake.h"

namespace Sexy {
	class Graphics;
	class GameApp;

	class Board : public Widget {
	public:
		typedef shared_ptr<Board> HardPtr;

		Board(Game::Snake::HardPtr snake, int syncRefreshRate);
		virtual void Draw(Graphics* g);
		virtual void UpdateF(float theFrac);
		virtual void KeyDown(KeyCode theKey);
	private:
		Game::Snake::HardPtr _snake;
		int _syncRefreshRate;
	};
}