#pragma once
#include "SexyAppFramework/Widget.h"

namespace Sexy {
	class Graphics;
	class GameApp;
	
	class Board : public Widget {
	public:
		Board(GameApp* theApp);
		virtual void Draw(Graphics* g);
		virtual void Update();
	private:
		GameApp* mApp;
	};
}