#pragma once
#include "Config.h"
#include "GameEntity.h"

//static int itemType = 0;
//static int itemTile = 0;
//static bool bItem = false;


class Image;
class Item;
class Tank;
class EnemyManager;
class ItemManager : public GameEntity
{
public:
	TILE_INFO tileInfo[TILE_COUNT_Y * TILE_COUNT_X];

	vector<Item*> vecItems;
	vector<Item*>::iterator itItems;

	POINT pos;

	Item* item;
	int itemCount = 0;

	HRESULT Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo);
	void Update();
	void Render(HDC hdc);
	void Release();
};

