#include "Item.h"
#include "Image.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "Tank.h"
#include "EnemyManager.h"
#include "ItemManager.h"

HRESULT Item::Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo, ItemManager* itemManager)
{
	itemImage[0] = ImageManager::GetSingleton()->FindImage("Image/Item/Item1.bmp");
	itemImage[1] = ImageManager::GetSingleton()->FindImage("Image/Item/Item2.bmp");
	itemImage[2] = ImageManager::GetSingleton()->FindImage("Image/Item/Item3.bmp");
	itemImage[3] = ImageManager::GetSingleton()->FindImage("Image/Item/Item4.bmp");
	itemImage[4] = ImageManager::GetSingleton()->FindImage("Image/Item/Item5.bmp");
	itemImage[5] = ImageManager::GetSingleton()->FindImage("Image/Item/Item6.bmp");
	itemImage[6] = ImageManager::GetSingleton()->FindImage("Image/Item/Item7.bmp");

	itemType = type;
	itemTile = tile;
	this->tank = tank;
	this->enemyMgr = enemyMgr;
	//this->stageInfo = stageInfo;
	rc.left = ((itemTile % 26) * 16) + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16;
	rc.top = ((itemTile / 26) * 16) + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y;
	rc.right = ((itemTile % 26) * 16) + 32 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16;
	rc.bottom = ((itemTile / 26) * 16) + 32 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y;

	this->itemManager = itemManager;

	return S_OK;

}

void Item::Update()
{

}

void Item::Render(HDC hdc)
{
	itemImage[itemType]->Render(hdc,
		((itemTile % 26) * 16) + 32 / 2 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16,
		((itemTile / 26) * 16) + 32 / 2 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y);
}

void Item::Release()
{
	for (int i = 0; i < 7; i++)
	{
		SAFE_DELETE(itemImage[i]);
	}
}

void Item::UseItem()
{

}

void Item::ItemPoint()
{
	GetItemManager()->SetItemPoint(true);
}

void HelmetItem::UseItem()
{
	((PlayerTank*)GetTank())->SetCheckShieldOn(true);
	GetTank()->SetElapsedCount(0.0f);
}

void ClockItem::UseItem()
{
	GetEnemyManager()->SetClockItem(true);
}

void ShovelItem::UseItem()
{
	GetItemManager()->Fortification();
}

void StarItem::UseItem()
{
	if (GetTank()->GetEnforceCount() != 3)
	{
		GetTank()->AddEnforceCount(1);
	}
}

void GrenadeItem::UseItem()
{
	GetEnemyManager()->BoomItem();
}

void TankLifeItem::UseItem()
{
	GameManager::GetSingleton()->player1Life++;
}
