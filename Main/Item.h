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
	Tank* tank;
	EnemyManager* enemyMgr;

	ItemManager* itemManager;
	
public:
	RECT rc;
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
	
};

class HelmetItem : public Item
{
public:
	virtual void UseItem() override;
};

class ClockItem : public Item
{
public:
	virtual void UseItem() override;
};
class ShovelItem : public Item
{
public:
	virtual void UseItem() override;
};
class StarItem : public Item
{
public:
	virtual void UseItem() override;
};
class GrenadeItem : public Item
{
public:
	virtual void UseItem() override;
};
class TankLifeItem : public Item
{
public:
	virtual void UseItem() override;
};