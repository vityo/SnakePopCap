#pragma once
#include "SexyAppFramework/SexyAppBase.h"

namespace Sexy {
	class Board;

	class GameApp : public SexyAppBase {
	public:
		GameApp();
		virtual ~GameApp();
		virtual void Init();
		virtual void LoadingThreadCompleted();
	private:
		Board* mBoard;
	};
}

