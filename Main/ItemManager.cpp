#include "ItemManager.h"
#include "Image.h"
#include "Item.h"
#include "Tank.h"
#include "EnemyManager.h"
#include "ItemFactorial.h"

HRESULT ItemManager::Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo)
{
	//itemCount = 10;
	//vecItems.reserve(itemCount);

	itemFactorial[0]= new HelmetItemFactorial;
	itemFactorial[1] = new ClockItemFactorial;
	itemFactorial[2] = new ShovelItemFactorial;
	itemFactorial[3] = new StarItemFactorial;
	itemFactorial[4] = new GrenadeItemFactorial;
	itemFactorial[5] = new TankLifeItemFactorial;



	Item* tempItem = itemFactorial[type]->CreateTank();
	tempItem->Init(type, tile, tank, enemyMgr, tileInfo, this);

	vecItems.push_back(tempItem);

	fortificationCount = 0;
	bIsFortification = false;

	this->tileInfo = tileInfo;
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
}

void ItemManager::Render(HDC hdc)
{
	for (itItems = vecItems.begin();
		itItems != vecItems.end(); itItems++)
	{
		(*itItems)->Render(hdc);
	}
}

void ItemManager::Release()
{

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
	cout << "진지구축!" << endl;
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

	cout << "진지구축 해제!" << endl;
	bIsFortification = false;
	fortificationCount = 0;
}


