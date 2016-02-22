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
	// ���� ������
	class Snake {
	public:
		typedef shared_ptr<Snake> HardPtr; // ������� ���������
		typedef uint8_t index_type; // ��� ��� ������� ������
		typedef pair<index_type, index_type> cell_type; // ��� ������
		enum Direction{LEFT, RIGHT, UP, DOWN}; // ���� ����������� ��������
		enum State { READY, GAME, END }; // ���� ��������� ����

		Snake(shared_ptr<Buffer> data, pair<int, int> screenSize); // �����������, ������ � ������� ����� � ������� ������
		virtual void Update(float dt); // dt ������� ������� � ���������� �����
		virtual void Draw(Graphics* g); // ���������

		State getState(); // ������ ������� ��������� ����
		void setState(State state); // ���������� ������� ��������� ����
		void setDirection(Direction direction); // ���������� ��������� ����������� ������
		void reset(); // �������� ����, ������ ������
		bool isGameOverTimer(); // ���� ������� �����������, �� ��� ������� ��������� + ����� ���������, ���� ����� ������, ��� ��������
	protected:
		typedef uint8_t pixel_type; // ��� ��� ��������� �������� (������ ���������, ������� �����)
		enum Prise { 
			SCORE = 0, // ����������� �������������� ����
			SLICE = 1, // �������� ������
			SPEED = 2, // �������� ��������
			PRISE_COUNT = 3 // ����������
		}; // ��������� �����

		void load(shared_ptr<Buffer> data); // ��������� ����� �� �����
		void makeCellFreeWithoutWallContainer(); // ������ ��������� ������ ������ ����� (�� �������� � �������� ����) �� ������ ���� 
		Point getTitlePosition(string title); // �������� ������� ��� ��������� �� ������ ��� ������
		void calcDrawSizes(pair<int, int> screenSize); // ��������� ��� ������� ��� ���������
		void next(); // ���������� ��� ������
		void setScore(long value); // ���������� ���������� ������������ �����
		void runPrise(Prise type); // ��������� ��, ��� ���� ����
		shared_ptr<cell_type> findCellFree(bool forPrise = false, bool withoutWallsOnly = false); // ����� ������ ��������� �����

		set<cell_type> _walls; // �����
		set<cell_type> _cellsFreeWithoutWalls; // ������ ������
		vector<vector<Point>> _wallsPoly; // ������ ����� ��� ������������� ��������� ����������� ���� (��������, ������ ���� �����)
		deque<cell_type> _snake; // ������ ������
		pixel_type _cellPixelSize; // ������ ������ � ������ � � ������
		Point _drawPosition; // ������� ��� ��������� ����� ���� ����
		pair<int, int> _direction; // ���� ������ ������ ������
		pair<float, float> _stepTimer; // ������ ��� ���� ������
		State _state; // ��������� ����
		Font* _font; // ����� ��� ������
		int _fontHeight; // ������ ������
		bool _titleDraw; // ��� �������� ���������
		string _title; // ����� ���������
		Point _titlePosition; // ������� ���������
		Point _titleStartPosition; // ������� ��������� ���������� ������
		Point _titleEndPosition; // ������� ��������� � ����� ����
		shared_ptr<cell_type> _food; // ������ ���
		shared_ptr<cell_type> _cellEndCaused; // ������, ������� �������� ����� ����
		Color _cellEndCausedColor; // ���� ��� ��������� ������, ������� �������� ����� ����
		
		Prise _priseType; // ��� �����
		Color _priseColor; // ���� ��������� �����
		uint8_t _foodCountBeforePrise; // ������� ���������� ��� ����� ���������� �����
		shared_ptr<cell_type> _prise; // ������ �����
		vector<Point> _priseTitlePositions; // ������� ���������� ���� ��� ��������������� ������
		pair<float, float> _priseTimer; // ������ ������������ �����
		bool _priseDraw; // ��� �������� �����

		long _score; // ������� ����, ����
		Point _scorePosition; // ������� ��� ��������� ������ �����
		pair<index_type, index_type> _gameSize; // ������ ������� ���� � ����������� �����
		float _dtStepCoefficient; // ����������� �������
		long _scoreCoefficient; // ����������� ���������� �����
		pair<float, float> _gameOverTimer; // ������ �������� ������ ��������� ��� ����� ����
	};
}