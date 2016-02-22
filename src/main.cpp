#pragma once
#include <memory>
#include "GameApp.h"
#include "Assert.h"

using namespace Sexy;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	try {
		gHInstance = hInstance;
		auto anApp = std::make_shared<GameApp>();
		Assert2(anApp, "Failed to create SexyAppBase");
		anApp->Init();
		anApp->Start();
	}
	catch (exception& exc) {
		//а вдруг приложение SexyAppBase не создалось, тогда SexyAppBase::Popup не подойдет;
		MessageBox(nullptr, exc.what(), "Error", 1);
	}

	return 0;
}