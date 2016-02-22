#pragma once
#include <memory>
#include "SexyAppFramework/SexyAppBase.h"
#include "Board.h"

using namespace std;

namespace Sexy {
	class GameResourceManager;
	class XMLParser;

	// ���������� ����
	class GameApp : public SexyAppBase {
	public:
		static GameApp* instance(); // ���������� ��������� �� ������������ ��������� ������

		GameApp(); // ����������� ��� ����������
		virtual ~GameApp(); // ����������
		// �� ������ �������� demo �� PopCap ����� ��������� �������
		virtual void Init(); // ������������� ����������
		virtual void LoadingThreadProc(); // ��������� �������� ��������
		virtual void LoadingThreadCompleted(); // ��������� �������� ����� ��������
	private:
		// ��������� ����������� � ������������
		GameApp(const GameApp&) = delete;
		GameApp& operator=(const GameApp&) = delete;

		static GameApp* _instance; // ��������� �� ������������ ���������
		Board::HardPtr _board; // ������ �� �������
		shared_ptr<Buffer> _dataInit; // ����� � ������� �����
	};
}

