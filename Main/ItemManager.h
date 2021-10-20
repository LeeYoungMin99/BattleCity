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
class ItemFactorial;
class ItemManager : public GameEntity
{
public:
	Image* itemScore;
	int itemTile = 0;
	vector<Item*> vecItems;
	vector<Item*>::iterator itItems;

	POINT pos;

	Item* item;
	int itemCount = 0;
	int elapsedcount;
	bool itemPoint;


	int fortificationCount;
	bool bIsFortification;
	ItemFactorial* itemFactorial[6];

	TILE_INFO* tileInfo;
	HRESULT Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo);
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fortification();
	void DestoryFortification();
	inline void SetItemPoint(bool itemPoint) { this->itemPoint = itemPoint; }
};

