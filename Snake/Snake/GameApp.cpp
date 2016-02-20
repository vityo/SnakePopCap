#include "GameApp.h"
#include "Board.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Common.h"

using namespace Sexy;

GameApp::GameApp() {
	mProdName = "Demo 1";
	mProductVersion = "1.0";
	mTitle = StringToSexyStringFast("SexyAppFramework: " + mProdName + " - " + mProductVersion);
	mRegKey = "PopCap\\SexyAppFramework\\Demo1";
	mWidth = 640;
	mHeight = 480;
	mBoard = NULL;
}

GameApp::~GameApp() {
	mWidgetManager->RemoveWidget(mBoard);
	delete mBoard;
}

void GameApp::Init() {
	SexyAppBase::Init();
}

void GameApp::LoadingThreadCompleted() {
	SexyAppBase::LoadingThreadCompleted();

	if (mLoadingFailed)
		return;

	mBoard = new Board(this);
	mBoard->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mBoard);
}