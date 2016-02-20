#pragma once
#include <memory>
#include "SexyAppFramework/SexyAppBase.h"
#include "Board.h"
#include "Snake.h"

using namespace std;

namespace Sexy {
	class GameApp : public SexyAppBase {
	public:
		static GameApp* instance();

		GameApp();
		virtual ~GameApp();
		virtual void Init();
		virtual void LoadingThreadProc();
		virtual void LoadingThreadCompleted();
	private:
		GameApp(const GameApp&) = delete;
		GameApp& operator=(const GameApp&) = delete;

		static GameApp* _instance;
		Board::HardPtr _board;
		Game::Snake::HardPtr _snake;
	};
}

