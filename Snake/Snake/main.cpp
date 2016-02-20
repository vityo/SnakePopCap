#include "GameApp.h"

using namespace Sexy;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	gHInstance = hInstance;
	GameApp* anApp = new GameApp();
	anApp->Init();
	anApp->Start();
	delete anApp;
	return 0;
}