#include "EnemyManager.h"
#include "Enemy.h"
#include "Tank.h"

HRESULT EnemyManager::Init(TILE_INFO* tile, Tank* playerTank)
{
	enemyMaxCount = 6;
	vecEnemys.reserve(enemyMaxCount);

	tileInfo = tile;
	this->playerTank = playerTank;

	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));

	this->tileInfo = tile;


	return S_OK;
}

void EnemyManager::Update()
{
	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end();)
	{
		(*itEnemys)->Update();

		if ((*itEnemys)->HP <= 0)
		{
			Tank* temp = (*itEnemys);
			itEnemys = vecEnemys.erase(itEnemys);
			delete temp;
		}
		else
		{
			itEnemys++;
		}
	}
}


void EnemyManager::Render(HDC hdc)
{
	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end(); itEnemys++)
	{
		(*itEnemys)->Render(hdc);
	}
}

void EnemyManager::Release()
{
	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end(); itEnemys++)
	{
		SAFE_RELEASE((*itEnemys));
	}
	vecEnemys.clear();
}

void EnemyManager::AddEnemy(Tank* tank, POINTFLOAT pos)
{
	tank->SetPos(pos);

	tank->Init(tileInfo,this,playerTank);
	elapsedcount++;
	if (elapsedcount == 1/*RANDOM_2(0, 16)*/)
	{
		tank->bItem = true;
	}
	vecEnemys.push_back(tank);
}
