#include "GameApp.h"
#include "stdafx.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Common.h"

using namespace Sexy;
using namespace Game;

GameApp* GameApp::_instance = nullptr;

GameApp::GameApp()
	:_board(nullptr)
	,_snake(nullptr)
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

void GameApp::LoadingThreadProc() {
	const string fileName = "level.txt";
	Assert(FileExists(fileName));
	shared_ptr<Buffer> data = make_shared<Buffer>();
	bool readResult = ReadBufferFromFile(fileName, data.get());
	Assert(readResult);
	_snake = make_shared<Snake>(data, make_pair(mWidth, mHeight));
	Assert(_snake);
}

void GameApp::LoadingThreadCompleted() {
	SexyAppBase::LoadingThreadCompleted();

	if (mLoadingFailed) {
		return;
	}

	_board = make_shared<Board>(_snake);
	Assert(_board);
	_board->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(_board.get());
}