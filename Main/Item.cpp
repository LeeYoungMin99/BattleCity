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
	string itemName = "Image/Item/Item";
	itemName += to_string(type+1);
	itemName += ".bmp";

	itemImage = ImageManager::GetSingleton()->FindImage(itemName.c_str());
	itemScore = ImageManager::GetSingleton()->FindImage("Image/Icon/Point.bmp");

	itemType = type;
	itemTile = tile;
	this->tank = tank;
	this->enemyMgr = enemyMgr;
	rc.left = ((itemTile % 26) * 16) + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16;
	rc.top = ((itemTile / 26) * 16) + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y;
	rc.right = ((itemTile % 26) * 16) + 32 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16;
	rc.bottom = ((itemTile / 26) * 16) + 32 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y;

	this->itemManager = itemManager;

	return S_OK;

}

void Item::Update()
{
	if (useItem)
	{
		elapsedcount++;
		if (elapsedcount >= 30)
		{
			GetItemManager()->UseItem();
			elapsedcount = 0;
		}
	}
}

void Item::Render(HDC hdc)
{
	if (!useItem)
	{
		itemImage->Render(hdc,
			((itemTile % 26) * 16) + 32 / 2 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16,
			((itemTile / 26) * 16) + 32 / 2 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y);
	}
	else
	{
		itemScore->Render(hdc,
			((itemTile % 26) * 16) + 32 / 2 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16,
			((itemTile / 26) * 16) + 32 / 2 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y,4 , 0);
	}
}

void Item::Release()
{
}

void Item::UseItem()
{

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
