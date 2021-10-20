#pragma once
#include "Config.h"
#include "GameObject.h"

class Image;
class Tank;
class EnemyManager;
class ItemManager;
class Item : public GameObject
{
private:
	Image* itemImage[7];
	int itemType = 0;
	int itemTile = 0;

	GameEntity* stageInfo = nullptr;
	Tank* tank;
	EnemyManager* enemyMgr;

	ItemManager* itemManager;
	
public:
	RECT rc;
	HRESULT Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo, ItemManager* itemManager);
	void Update();
	void Render(HDC hdc);
	void Release();
	void UseItem();
	
	inline int GetType() { return this->itemType; }
};

