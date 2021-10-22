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
	Image* itemImage;
	int itemType = 0;
	int itemTile = 0;
	int elapsedcount;

	GameEntity* stageInfo = nullptr;
	Tank* tank = nullptr;
	EnemyManager* enemyMgr = nullptr;

	ItemManager* itemManager = nullptr;
	
public:
	RECT rc = {};
	HRESULT Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo, ItemManager* itemManager);
	void Update();
	void Render(HDC hdc);
	void Release();
	virtual void UseItem() = 0;
	void ItemPoint();

	inline Tank* GetTank() { return tank; }
	inline EnemyManager* GetEnemyManager() { return enemyMgr; };
	inline int GetType() { return this->itemType; }
	inline ItemManager* GetItemManager() { return itemManager; }
	
	Item() {};
	virtual ~Item() {} 
};

class HelmetItem : public Item
{
public:
	virtual void UseItem() override;

	HelmetItem() {};
	virtual ~HelmetItem() {}
};

class ClockItem : public Item
{
public:
	virtual void UseItem() override;

	ClockItem() {};
	virtual ~ClockItem() {}
};

class ShovelItem : public Item
{
public:
	virtual void UseItem() override;

	ShovelItem() {};
	virtual ~ShovelItem() {}
};

class StarItem : public Item
{
public:
	virtual void UseItem() override;

	StarItem() {};
	virtual ~StarItem() {}
};

class GrenadeItem : public Item
{
public:
	virtual void UseItem() override;

	GrenadeItem() {};
	virtual ~GrenadeItem() {}
};

class TankLifeItem : public Item
{
public:
	virtual void UseItem() override;

	TankLifeItem() {};
	virtual ~TankLifeItem() {}
};