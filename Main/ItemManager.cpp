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



	

	fortificationCount = 0;
	bIsFortification = false;

    return S_OK;
}

void ItemManager::Update()
{
	for (int i = 0; i < vecItems.size(); ++i)
	{
		vecItems[i]->Update();
	}

	if (bIsFortification)
	{
		fortificationCount++;
		if (fortificationCount > 300)
		{
			DestoryFortification();
		}
	}
	
}

void ItemManager::Render(HDC hdc)
{
	for (int i = 0; i < vecItems.size(); ++i)
	{
		vecItems[i]->Render(hdc);
	}
}

void ItemManager::Release()
{
	for (int i=0; i<6; i++)
	{
		SAFE_DELETE(itemFactory[i]);
	}

	for (int i = 0; i < vecItems.size(); ++i)
	{
		SAFE_RELEASE(vecItems[i]);
	}
	vecItems.clear();

	tileInfo = nullptr;
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

	vecItems.push_back(tempItem);
	this->tileInfo = tileInfo;
}

void ItemManager::UseItem()
{
	for (int i = 0; i < vecItems.size(); ++i)
	{
		if (vecItems[i]->GetUseItem())
		{
			SAFE_DELETE(vecItems[i]);
			vecItems.erase(vecItems.begin() + i);

			/*Item* tempItem = (*itItemList);
			itItemList = ItemList->erase(itItemList);
			SAFE_DELETE(tempItem);*/
		}
	}
}


