#pragma once
#include <set>
#include <memory>
#include <queue>
#include "SexyAppFramework/Buffer.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/Point.h"

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
		enum EventType {
			EVENT_NONE, // ������ �� ���������
			EVENT_WALL, // ��������� � �����
			EVENT_SNAKE, // ��������� � ����
			EVENT_PRISE_CREATE, // �������� ����
			EVENT_PRISE_EAT, // ����� ����
			EVENT_FOOD // c���� ���
		}; // ��������� ������� ����� ���� ������
		struct PriseCell {
			cell_type position; // ������� �����
			PriseType type; // ��� �����
			PriseCell(cell_type _position, PriseType _type) : position(_position), type(_type) {}
		};

		Snake(cell_type wallsSize, set<cell_type> wallsInner); // �����������. ������� �������� ����, � ����� ����������
		void start(); // �������� ����
		EventType next(); // ���������� ��� ������. true, ���� ����������� ��������
		void reset(); // �������� ����, ������ ������
		// ��� ��� �� ���� �� ������� �������� ����� ��������� �������� �����:
		void priseActiveEnd(); // ��������� �������� ��������� �����
		void priseDisappear(); // ������� ���� � ����

		State getState(); // ������ ������� ��������� ����
		void setDirection(Direction direction); // ���������� ��������� ����������� ������
		pair<index_type, index_type> getGameSize(); // ������� ������ ������� ����
		cell_type getWallsSize(); // ������� ������� �������� ����
		set<cell_type>& getWalls(); // ������� ������ �� �������
		deque<cell_type>& getSnake(); // ������� ������ ������
		shared_ptr<cell_type> getFood(); // ������� ��������� ������ ���
		shared_ptr<PriseCell> getPrise(); // ������� ���� �� ���� ����
		shared_ptr<cell_type> get�ellEndCaused(); // ������� ������, ������� �������� ������
		EventType getEventLast(); // ������ ��������� �������
		long getScore(); // �������� ������� ����
		PriseType getPriseTypeActive(); // �������� �������������� ��� �����
	protected:
		void makeCellFreeWithoutWallContainer(); // ������ ��������� ������ ������ ����� (�� �������� � �������� ����) �� ������ ���� 
		shared_ptr<cell_type> findCellFree(bool forPrise = false, bool withoutWallsOnly = false); // ����� ������ ��������� �����
		void setPriseTypeActive(PriseType value); // ���������� ��� ��������� �����
		void slice(int count); // �������� � ������

		cell_type _wallsSize; // ������� �������� ����
		set<cell_type> _walls; // ���������� �����
		set<cell_type> _cellsFreeWithoutWalls; // ������ ������
		deque<cell_type> _snake; // ������ ������
		pair<int, int> _direction; // ���� ������ ������ ������
		State _state; // ��������� ����
		shared_ptr<cell_type> _food; // ������ ���
		shared_ptr<cell_type> _cellEndCaused; // ������, ������� �������� ����� ����
		
		PriseType _priseTypeActive; // ������� ��� ��������� �����
		uint8_t _foodCountBeforePrise; // ������� ���������� ��� ����� ���������� �����
		uint8_t _priseSameTypeCountLast; // ������� ��� ��� ���� � ��� �� ��� �����
		PriseType _priseTypeActiveLast; // ��������� ��� �����, ������� ���
		shared_ptr<PriseCell> _prise; // ����
		vector<Point> _priseTitlePositions; // ������� ���������� ���� ��� ��������������� ������

		long _score; // ������� ����, ����
		pair<index_type, index_type> _gameSize; // ������ ������� ���� � ����������� �����
		long _scoreCoefficient; // ����������� ���������� �����
		EventType _eventLast; // ��������� ����������� �������
	};
}