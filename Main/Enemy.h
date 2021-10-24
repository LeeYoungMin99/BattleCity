#pragma once
#include "Config.h"
#include "GameObject.h"

class AmmoManager;
class Enemy : public GameObject
{
private:
	int elapsedCount = 0;
	AmmoManager* ammoMgr = nullptr;

	int fireDelay = 0;
	int fireTimer = 0;

public:
	virtual ~Enemy() = default;

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();
	
};

