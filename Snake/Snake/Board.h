#pragma once
#include "SexyAppFramework/Widget.h"

namespace Sexy {
	class Graphics;
	class GameApp;

	class Board : public Widget {
	public:
		virtual void Draw(Graphics* g);
		virtual void Update();
	};
}