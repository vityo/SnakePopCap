#pragma once
#include <memory>
#include "SexyAppFramework/SexyAppBase.h"
#include "Board.h"

using namespace std;

namespace Sexy {
	class GameResourceManager;

	// приложение игры
	class GameApp : public SexyAppBase {
	public:
		static GameApp* instance(); // возвращает указатель на единственный экземпл€р класса

		GameApp(); // конструктор без параметров
		virtual ~GameApp(); // деструктор
		virtual void Init(); // инициализаци€ приложени€
		virtual void LoadingThreadProc(); // процедура загрузки ресурсов
		virtual void LoadingThreadCompleted(); // выполн€ем действи€ после загрузки
	private:
		// запрещаем копирование и присваивание
		GameApp(const GameApp&) = delete;
		GameApp& operator=(const GameApp&) = delete;

		static GameApp* _instance; // указатель на единственный экземпл€р
		Board::HardPtr _board; // виджет со змейкой
		shared_ptr<Buffer> _dataInit; // буфер с данными карты
	};
}

