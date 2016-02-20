#pragma once
#include <memory>
#include "SexyAppFramework/SexyAppBase.h"

using namespace std;

namespace Sexy {
	class Board;

	class GameApp : public SexyAppBase {
	public:
		static GameApp* instance();

		GameApp();
		virtual ~GameApp();
		virtual void Init();
		virtual void LoadingThreadCompleted();
	private:
		static GameApp* _instance;
		shared_ptr<Board> _board;
	};
}

