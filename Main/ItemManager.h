#pragma once
#include "Config.h"
#include "GameEntity.h"
 

class Image;
class Item;
class Tank;
class TankManager;
class ItemFactory;
class ItemManager : public GameEntity
{
private:
	vector<Item*> vecItems = {};
	POINT pos = {};
	Item* item = nullptr;

	int fortificationCount = 0;
	bool bIsFortification = false;
	ItemFactory* itemFactory[6] = {};
	TILE_INFO* tileInfo = nullptr;

public:
	virtual ~ItemManager() = default;

	HRESULT Init(TILE_INFO* tileInfo);
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fortification();
	void DestoryFortification();
	void CreateItem(int type, int tile, Tank* tank, TankManager* tankManager);
	void UseItem();

	inline vector<Item*>* GetAddressVecItem() { return &(this->vecItems); }
	inline vector<Item*> GetVecItem() { return this->vecItems; }
};

