#pragma once
#include <set>
#include <memory>
#include <queue>
#include "SexyAppFramework/Buffer.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/Point.h"
#include "SexyAppFramework/ImageFont.h"

using namespace std;
using namespace Sexy;

namespace Game {
	class Snake {
	public:
		typedef shared_ptr<Snake> HardPtr;
		typedef uint8_t index_type;
		typedef pair<index_type, index_type> cell_type;
		enum Direction{LEFT, RIGHT, UP, DOWN};
		enum State { READY, GAME, END };

		Snake(shared_ptr<Buffer> data, pair<int, int> screenSize);
		virtual void Update(float dt);
		virtual void Draw(Graphics* g);

		State getState();
		void setState(State state);
		void setDirection(Direction direction);
		void reset();
	protected:
		typedef uint8_t pixel_type;

		void load(shared_ptr<Buffer> data);
		void makeCellFreeWithoutWallContainer();
		void calcDrawSizes(pair<int, int> screenSize);
		void next();
		void setScore(long value);
		shared_ptr<cell_type> findCellFree(bool withoutWallsOnly = false);

		set<cell_type> _walls;
		set<cell_type> _cellsFreeWithoutWalls;
		vector<vector<Point>> _wallsPoly;
		deque<cell_type> _snake;
		pixel_type _cellPixelSize;
		Point _drawPosition;
		pair<int, int> _direction;
		pair<float, float> _stepTimer;
		State _state;
		Font* _font;
		string _title;
		Point _titlePosition;
		Point _titleStartPosition;
		Point _titleEndPosition;
		shared_ptr<cell_type> _food;
		shared_ptr<cell_type> _cellEndCaused;
		Color _cellEndCausedColor;
		long _score;
		Point _scorePosition;
		pair<index_type, index_type> _gameSize;
	};
}