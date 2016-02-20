#include "Board.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/Color.h"
#include "SexyAppFramework/Point.h"

using namespace Sexy;

Board::Board(GameApp* theApp) {
	mApp = theApp;
}

void Board::Update() {
	Widget::Update();
	MarkDirty();
}

void Board::Draw(Graphics* g) {
	g->FillRect(0, 0, mWidth, mHeight);
	g->SetColor(Color(255, 128, 64));
	g->FillRect(mWidth / 2, mHeight / 2, 50, 50);
	g->SetColor(Color(255, 0, 0));
	g->DrawLine(0, 0, 200, 150);
	g->SetColor(Color(0, 255, 0));
	g->DrawLine(mWidth, 0, mWidth - 200, 150);
	g->SetColor(Color(0, 0, 255));
	g->DrawLine(0, mHeight, 200, mHeight - 150);
	g->SetColor(Color(255, 255, 255));
	g->DrawLine(mWidth, mHeight, mWidth - 200, mHeight - 150);
	g->SetColor(Color(255, 0, 255));
	g->DrawRect(200, 150, (mWidth - 200) - 200, (mHeight - 150) - 150);
	Point trianglePoints[3];
	trianglePoints[0] = Point(30, 30);
	trianglePoints[1] = Point(30, 60);
	trianglePoints[2] = Point(60, 45);
	g->SetColor(Color(255, 255, 0));
	g->PolyFill(trianglePoints, 3);
	g->SetColor(Color(0, 255, 255));
	Point pentaPoints[5];
	pentaPoints[0] = Point(200, 0);
	pentaPoints[1] = Point(150, 40);
	pentaPoints[2] = Point(150, 80);
	pentaPoints[3] = Point(250, 80);
	pentaPoints[4] = Point(250, 40);
	g->PolyFill(pentaPoints, 5);
}