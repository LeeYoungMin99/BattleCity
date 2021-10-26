#pragma once
#include "GameEntity.h"

class TankManager;
class Tank;
class Image;
class BattleScene : public GameEntity
{
private:
	// �÷��̾� ����
	Tank* rocket;

	// �� UFO
	TankManager* tankMgr;

	// ��� �̹���
	Image* backGround;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);	// �����ε�
	void Release();
};

