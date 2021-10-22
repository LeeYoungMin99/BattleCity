#pragma once
#include "Config.h"
#include "GameEntity.h"

// 우리가 구성할 게임 월드
class Image;
class Tank;
class TankFactory;
class MainGame : public GameEntity
{
private:
	// 타이머
	HANDLE hTimer = nullptr;

	// UI
	//char text[128];

	int mousePosX = 0;
	int mousePosY = 0;
	int clickedMousePosX = 0;
	int clickedMousePosY = 0;

	// 백버퍼
	Image* backBuffer = nullptr;
	 

public:
	virtual ~MainGame() {}

	HRESULT Init();
	void Update();
	//void Render();			// 오버라이딩 : 상속 관계에서 부모클래스의 함수를 재정의
	void Render(HDC hdc);	// 오버로딩
	void Release();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

//int MainGame::clickedMousePosX = 0;