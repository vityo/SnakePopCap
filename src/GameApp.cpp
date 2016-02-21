#include "stdafx.h"
#include "GameApp.h"
#include "SexyAppFramework/ResourceManager.h"
#include "Snake.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Common.h"

using namespace Sexy;
using namespace Game;

GameApp* GameApp::_instance = nullptr;

GameApp::GameApp()
	:SexyAppBase()
	,_board(nullptr)
	,_dataInit(nullptr)
{ 
	Assert2(_instance == nullptr, "GameApp is singleton");
	_instance = this;

	mProdName = "Snake";
	mProductVersion = "1.0";
	mTitle = "SexyAppFramework: " + mProdName + " - " + mProductVersion;
	mRegKey = "PopCap\\SexyAppFramework\\Snake";
	mWidth = 640;
	mHeight = 480;
	mVSyncUpdates = true;
}

GameApp::~GameApp() {
	mWidgetManager->RemoveWidget(_board.get());
}

void GameApp::Init() {
	SexyAppBase::Init();

	LoadResourceManifest();

	if (!mResourceManager->LoadResources("Init")) {
		mLoadingFailed = true;
		ShowResourceError(true);

		return;
	}
}

GameApp* GameApp::instance() {
	return _instance;
}

void GameApp::LoadingThreadProc() {
	const string fileName = "card.txt";
	Assert2(FileExists(fileName), "Failed to find card file");
	_dataInit = make_shared<Buffer>();
	bool readResult = ReadBufferFromFile(fileName, _dataInit.get());
	Assert2(readResult, "Failed to read card file");
	_font = make_shared<ImageFont>(this, "font/Kiloton9.txt");
	Assert2(_font->mFontData->mInitialized, "Failed to load font");
}

void GameApp::LoadingThreadCompleted() {
	SexyAppBase::LoadingThreadCompleted();

	if (mLoadingFailed) {
		return;
	}

	Snake::HardPtr snake = make_shared<Snake>(_dataInit, make_pair(mWidth, mHeight));
	Assert(snake);
	_board = make_shared<Board>(snake, mSyncRefreshRate);
	Assert(_board);
	_board->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(_board.get()); 
	mWidgetManager->SetFocus(_board.get());
}