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
	// ���� ������. ������ ���������� ������
	class Snake {
	public:
		typedef shared_ptr<Snake> HardPtr; // ������� ���������
		typedef uint8_t index_type; // ��� ��� ������� ������
		typedef pair<index_type, index_type> cell_type; // ��� ������
		enum Direction{LEFT, RIGHT, UP, DOWN}; // ���� ����������� ��������
		enum State { READY, GAME, END }; // ���� ��������� ����
		enum PriseType { 
			SCORE = 0, // ����������� �������������� ����
			SLICE = 1, // �������� ������
			SPEED = 2, // �������� ��������
			PRISE_COUNT = 3, // ����������
			PRISE_NONE = 4 // ���� �� ���������� ������� �������� ����
		}; // ��������� �����
		enum EndCaused {
			CAUSED_NONE, // ���� �� �����������
			WALL, // ��������� � �����
			SNAKE // ��������� � ����
		}; // ������� ���������
		struct PriseCell {
			cell_type position; // ������� �����
			PriseType type; // ��� �����
			bool expired; // ������� ��������� ������ �����
			PriseCell(cell_type _position, PriseType _type) : position(_position), type(_type), expired(false) {}
		};

		Snake(cell_type wallsSize, shared_ptr<Buffer> data); // �����������. ������� ����
		void start(); // �������� ����
		bool next(); // ���������� ��� ������. true, ���� ����������� ��������
		void reset(); // �������� ����, ������ ������

		State getState(); // ������ ������� ��������� ����
		void setDirection(Direction direction); // ���������� ��������� ����������� ������
		pair<index_type, index_type> getGameSize(); // ������� ������ ������� ����
		cell_type getWallsSize(); // ������� ������� ���������� ����
		set<cell_type> getWalls(); // ������� ������ �� �������
		deque<cell_type> getSnake(); // ������� ������ ������
		shared_ptr<cell_type> getFood(); // ������� ��������� ������ ���
		shared_ptr<PriseCell> getPrise(); // ������� ���� �� ���� ����
		shared_ptr<cell_type> get�ellEndCaused(); // ������� ������, ������� �������� ������
		EndCaused getEndCaused(); // ������ ������� ������
		long getScore(); // �������� ������� ����
		PriseType getPriseTypeActive(); // �������� �������������� ��� �����
	protected:
		void load(shared_ptr<Buffer> data); // ��������� ����� �� �����
		void makeCellFreeWithoutWallContainer(); // ������ ��������� ������ ������ ����� (�� �������� � �������� ����) �� ������ ���� 
		shared_ptr<cell_type> findCellFree(bool forPrise = false, bool withoutWallsOnly = false); // ����� ������ ��������� �����

		cell_type _wallsSize; // ������� ����
		set<cell_type> _walls; // �����
		set<cell_type> _cellsFreeWithoutWalls; // ������ ������
		deque<cell_type> _snake; // ������ ������
		pair<int, int> _direction; // ���� ������ ������ ������
		State _state; // ��������� ����
		shared_ptr<cell_type> _food; // ������ ���
		shared_ptr<cell_type> _cellEndCaused; // ������, ������� �������� ����� ����
		
		PriseType _priseTypeActive; // ������� ��� ��������� �����
		uint8_t _foodCountBeforePrise; // ������� ���������� ��� ����� ���������� �����
		shared_ptr<PriseCell> _prise; // ����
		vector<Point> _priseTitlePositions; // ������� ���������� ���� ��� ��������������� ������

		long _score; // ������� ����, ����
		pair<index_type, index_type> _gameSize; // ������ ������� ���� � ����������� �����
		long _scoreCoefficient; // ����������� ���������� �����
		EndCaused _endCaused; // ������� ���������
	};
}