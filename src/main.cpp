#pragma once
#include <memory>
#include "GameApp.h"

using namespace Sexy;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	try {
		gHInstance = hInstance;
		auto anApp = std::make_shared<GameApp>();

		if (anApp) {
			anApp->Init();
			anApp->Start();
		}
	}
	catch (exception& exc) {
		MessageBox(nullptr, exc.what(), "Exception", 1);
	}

	return 0;
}