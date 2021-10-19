#pragma once
#include "Config.h"
#include "GameObject.h"

class Image;
class Tank;
class EnemyManager;
class Item : public GameObject
{
private:
	Image* itemImage[7];
	int itemType = 0;
	int itemTile = 0;

	GameEntity* stageInfo = nullptr;
	Tank* tank;
	EnemyManager* enemyMgr;
public:
	RECT rc;
	HRESULT Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo);
	void Update();
	void Render(HDC hdc);
	void Release();
	void UseItem();
	
	inline int GetType() { return this->itemType; }
};

