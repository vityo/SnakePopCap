#include "stdafx.h"
#include "Snake.h"
#include "SexyAppFramework/Color.h"
#include "SexyAppFramework/Point.h"

using namespace Game;

const Snake::cell_type gameInnerSize(16, 13);
const Snake::cell_type wallsSize(1, 1);

Snake::Snake(shared_ptr<Buffer> data, pair<float, float> screenSize)
{ 
	load(data);

	float screenAspect = screenSize.first / screenSize.second;
	pair<float, float> gameSize = make_pair(gameInnerSize.first + wallsSize.first * 2, gameInnerSize.second + wallsSize.second * 2);
	float gameAspect = gameSize.first / gameSize.second;

	if (screenAspect > gameAspect) {
		_cellPixelSize = static_cast<pixel_type>(screenSize.second / gameSize.second);
		_drawPosition = make_shared<Point>(static_cast<int>((screenSize.first - _cellPixelSize * gameSize.first) * 0.5f), 0);
	}
	else {
		_cellPixelSize = screenSize.first / gameSize.first;
		_drawPosition = make_shared<Point>(0, static_cast<int>((screenSize.second - _cellPixelSize * gameSize.second) * 0.5f));
	}
}

void Snake::load(shared_ptr<Buffer> data) {
	// читаем файл карты со стенами. по максимуму используем функции PopCap, которые работают, ничего в нем не меняя. Например, ReadBuffer, ReadString - уже падают
	ByteVector byteVector;
	const uint8_t symbols = 2; // перенос строки и возврат каретки
	byteVector.assign((gameInnerSize.first + symbols) * gameInnerSize.second - symbols, (uchar)0);
	data->ReadBytes(&byteVector[0], data->GetDataLen());

	uint8_t wallXIndex = wallsSize.first;
	uint8_t wallYIndex = wallsSize.second;

	for (uchar ch : byteVector) {
		if (ch != '\n' && ch != '\r') {
			if (ch != ' ') {
				_walls.insert(cell_type(wallXIndex, wallYIndex));
			}

			++wallXIndex;

			if (wallXIndex == gameInnerSize.first + wallsSize.first) {
				++wallYIndex;
				wallXIndex = wallsSize.first;
			}
		}
	}
}

void Snake::Draw(Graphics* g) {
	g->SetColor(Color(255, 0, 0));
	for (auto wall : _walls) {
		g->FillRect(_drawPosition->mX + wall.first * _cellPixelSize, _drawPosition->mY + wall.second * _cellPixelSize, _cellPixelSize, _cellPixelSize);
	}
}