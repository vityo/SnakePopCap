#include "GameApp.h"
#include "stdafx.h"
#include "Board.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Common.h"

using namespace Sexy;

GameApp* GameApp::_instance = nullptr;

GameApp::GameApp()
	:_board(nullptr)
{ 
	Assert2(_instance == nullptr, "GameApp is singleton");
	_instance = this;

	mProdName = "Snake";
	mProductVersion = "1.0";
	mTitle = "SexyAppFramework: " + mProdName + " - " + mProductVersion;
	mRegKey = "PopCap\\SexyAppFramework\\Snake";
	mWidth = 640;
	mHeight = 480;
}

GameApp::~GameApp() {
	mWidgetManager->RemoveWidget(_board.get());
}

void GameApp::Init() {
	SexyAppBase::Init();
}

GameApp* GameApp::instance() {
	return _instance;
}

void GameApp::LoadingThreadCompleted() {
	SexyAppBase::LoadingThreadCompleted();

	if (mLoadingFailed) {
		return;
	}

	_board = make_shared<Board>();
	Assert(_board);
	_board->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(_board.get());
}