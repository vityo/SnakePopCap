#include "stdafx.h"
#include "GameApp.h"
#include "SexyAppFramework/ResourceManager.h"
#include "SexyAppFramework/XMLParser.h"
#include "Snake.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Common.h"
#include "GameInfo.h"

using namespace Sexy;
using namespace Game;

GameApp* GameApp::_instance = nullptr;

GameApp::GameApp()
	: SexyAppBase()
	, _board(nullptr)
	, _dataInit(nullptr)
{ 
	Assert2(_instance == nullptr, "GameApp is singleton");
	_instance = this;

	mProdName = "Snake";
	mProductVersion = "1.0";
	mTitle = "SexyAppFramework: " + mProdName + " - " + mProductVersion;
	mRegKey = "PopCap\\SexyAppFramework\\Snake";
	mWidth = 640;
	mHeight = 480;
	// разрешаем UpdateF функцию
	mVSyncUpdates = true;
}

GameApp::~GameApp() {
	mWidgetManager->RemoveWidget(_board.get());
	mResourceManager->DeleteResources("");
}

void GameApp::Init() {
	SexyAppBase::Init();

	// загружаем файл с описанием ресурсов игры
	LoadResourceManifest();

	// загружаем группу ресурсов Init
	if (!mResourceManager->LoadResources("Init")) {
		// если неудача - подробное описание ошибки
		mLoadingFailed = true;
		ShowResourceError(true);

		return;
	}

	shared_ptr<XMLParser> parser = make_shared<XMLParser>();

	if (!parser->OpenFile("properties/gameinfo.xml"))
	{
		mLoadingFailed = true;
		MsgBox(StrFormat("Couldn't open properties/gameinfo.xml. Error returned:\n\"%s\"",
			parser->GetErrorText().c_str()), "Error");
		return;
	}

	gameInfo.ExtractXMLData(parser);
}

GameApp* GameApp::instance() {
	return _instance;
}

void GameApp::LoadingThreadProc() {
	// нужно загрузить собственный файл
	// загружаем файл карты
	const string fileName = "card.txt";
	Assert2(FileExists(fileName), "Failed to find card file");
	_dataInit = make_shared<Buffer>();
	bool readResult = ReadBufferFromFile(fileName, _dataInit.get());
	Assert2(readResult, "Failed to read card file");
}

void GameApp::LoadingThreadCompleted() {
	SexyAppBase::LoadingThreadCompleted();

	if (mLoadingFailed) {
		return;
	}

	// создаем виджет со змейкой
	_board = make_shared<Board>(mSyncRefreshRate, make_pair(mWidth, mHeight), _dataInit);
	Assert(_board);
	_board->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(_board.get()); 
	mWidgetManager->SetFocus(_board.get()); // после установки фокуса, приходят нажатия клавиш
}