#include "ItemManager.h"
#include "Image.h"
#include "Item.h"
#include "Tank.h"
#include "EnemyManager.h"
#include "ItemFactory.h"

HRESULT ItemManager::Init()
{
	itemFactory[0] = new HelmetItemFactory;
	itemFactory[1] = new ClockItemFactory;
	itemFactory[2] = new ShovelItemFactory;
	itemFactory[3] = new StarItemFactory;
	itemFactory[4] = new GrenadeItemFactory;
	itemFactory[5] = new TankLifeItemFactory;


	itemScore = ImageManager::GetSingleton()->FindImage("Image/Icon/Point.bmp");
	itemPoint = false;

	fortificationCount = 0;
	bIsFortification = false;

    return S_OK;
}

void ItemManager::Update()
{
	if (bIsFortification)
	{
		fortificationCount++;
		if (fortificationCount > 300)
		{
			DestoryFortification();
		}
	}
	if (itemPoint)
	{
		elapsedcount++;
		if (elapsedcount >= 40)
		{
			elapsedcount = 0;
			itemPoint = false;
		}
	}
	
}

void ItemManager::Render(HDC hdc)
{
	for (itItems = vecItems.begin();
		itItems != vecItems.end(); itItems++)
	{
		(*itItems)->Render(hdc);
	}

	if (itemPoint)
	{
		itemScore->Render(hdc,
			((itemTile % 26) * 16) + 32 / 2 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16,
			((itemTile / 26) * 16) + 32 / 2 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y, 4, 0);
	}
}

void ItemManager::Release()
{
	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(itemFactory[i])
	}
}

void ItemManager::Fortification()
{
	int nexusIndex = 0;
	for (int i = 0; i < TILE_COUNT_X * TILE_COUNT_Y; i++)
	{

		if (tileInfo[i].tileType == TileType::Nexus)
		{
			nexusIndex = i;

			break;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j > 0 && tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].tileType != TileType::Nexus)
			{
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].frameX = 1;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].frameY = 0;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].tileType = TileType::Wall;

				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].collider.left = tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].rc.left + STAGE_SIZE_X;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].collider.right = tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].rc.right + STAGE_SIZE_X;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].collider.top = tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].rc.top + STAGE_SIZE_Y;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].collider.bottom = tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].rc.bottom + STAGE_SIZE_Y;

				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].bottomHit = 0;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].leftHit = 0;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].rightHit = 0;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].topHit = 0;
			}
		}
	}
	bIsFortification = true;
}

void ItemManager::DestoryFortification()
{
	int nexusIndex = 0;
	for (int i = 0; i < TILE_COUNT_X * TILE_COUNT_Y; i++)
	{

		if (tileInfo[i].tileType == TileType::Nexus)
		{
			nexusIndex = i;

			break;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j > 0 && tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].tileType != TileType::Nexus)
			{
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].frameX = 0;
				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].frameY = 0;

				tileInfo[nexusIndex + (TILE_COUNT_X * (i - 1)) - 1 + j].tileType = TileType::Brick;
			}
		}
	}

	bIsFortification = false;
	fortificationCount = 0;
}

void ItemManager::CreateItem(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo)
{
	Item* tempItem = itemFactory[type]->CreateTank();
	tempItem->Init(type, tile, tank, enemyMgr, tileInfo, this);

	itemTile = tile;
	vecItems.push_back(tempItem);
	this->tileInfo = tileInfo;
	
	
	
}


