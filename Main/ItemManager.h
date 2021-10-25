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
	vector<Item*> vecItems;
	POINT pos = {};
	Item* item = nullptr;

	int fortificationCount = 0;
	bool bIsFortification = false;
	ItemFactory* itemFactory[6] = {};
	TILE_INFO* tileInfo = nullptr;

public:
	virtual ~ItemManager() = default;

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fortification();
	void DestoryFortification();
	void CreateItem(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo);
	void UseItem();

	inline vector<Item*>* GetAddressVecItem() { return &(this->vecItems); }
	inline vector<Item*> GetVecItem() { return this->vecItems; }
};

