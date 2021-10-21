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
class ItemFactory;
class ItemManager : public GameEntity
{
private:
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
	ItemFactory* itemFactory[6];

	TILE_INFO* tileInfo;


public:
	
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fortification();
	void DestoryFortification();
	void CreateItem(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo);
	inline void SetItemPoint(bool itemPoint) { this->itemPoint = itemPoint; }

	inline vector<Item*>* GetAddressVecItem() { return &(this->vecItems); }
	inline vector<Item*> GetVecItem() { return this->vecItems; }
};

