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
	ImageManager::GetSingleton()->AddImage("Image/Item/Item1.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item2.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item3.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item4.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item5.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item6.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item7.bmp", 32, 32, true, RGB(255, 0, 255));
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
	GetTank()->bCheckShieldOn = true;
	GetTank()->elapsedCount = 0;
	
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
	if (!GetTank()->enforceCount == 4)
	{
		GetTank()->enforceCount++;
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
