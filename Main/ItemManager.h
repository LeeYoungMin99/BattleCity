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

	vector<Item*> vecItems;
	vector<Item*>::iterator itItems;

	POINT pos;

	Item* item;
	int itemCount = 0;

	int fortificationCount;
	bool bIsFortification;

	TILE_INFO* tileInfo;
	HRESULT Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo);
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fortification();
	void DestoryFortification();
};

