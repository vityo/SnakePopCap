#pragma once
#include <set>
#include <memory>
#include "SexyAppFramework/Buffer.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/Point.h"

using namespace std;
using namespace Sexy;

namespace Game {
	class Snake {
	public:
		typedef shared_ptr<Snake> HardPtr;
		typedef pair<uint8_t, uint8_t> cell_type;
		
		Snake(shared_ptr<Buffer> data, pair<float, float> screenSize);
		virtual void Draw(Graphics* g);
	private:
		typedef uint8_t pixel_type;
		void load(shared_ptr<Buffer> data);

		set<cell_type> _walls;
		pixel_type _cellPixelSize;
		shared_ptr<Point> _drawPosition;
	};
}