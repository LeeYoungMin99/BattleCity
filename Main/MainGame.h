#pragma once
#include "Config.h"
#include "GameEntity.h"

// �츮�� ������ ���� ����
class Image;
class Tank;
class TankFactory;
class MainGame : public GameEntity
{
private:
	// Ÿ�̸�
	HANDLE hTimer = nullptr;

	// UI
	//char text[128];

	int mousePosX = 0;
	int mousePosY = 0;
	int clickedMousePosX = 0;
	int clickedMousePosY = 0;

	// �����
	Image* backBuffer = nullptr;
	 

public:
	virtual ~MainGame() {}

	HRESULT Init();
	void Update();
	//void Render();			// �������̵� : ��� ���迡�� �θ�Ŭ������ �Լ��� ������
	void Render(HDC hdc);	// �����ε�
	void Release();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

//int MainGame::clickedMousePosX = 0;