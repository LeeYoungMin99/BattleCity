#include "ItemManager.h"
#include "Image.h"
#include "Item.h"
#include "Tank.h"
#include "EnemyManager.h"


HRESULT ItemManager::Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo)
{
	//itemCount = 10;
	//vecItems.reserve(itemCount);

	Item* tempItem = new Item;
	tempItem->Init(type, tile, tank, enemyMgr, tileInfo);
	
	vecItems.push_back(tempItem);

    return S_OK;
}

void ItemManager::Update()
{

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

