#pragma once
#include "Config.h"
#include "GameEntity.h"

// 우리가 구성할 게임 월드
class Image;
class MainGame : public GameEntity
{
private:
	// 타이머
	HANDLE hTimer = nullptr;

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
	void Render(HDC hdc);
	void Release();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
