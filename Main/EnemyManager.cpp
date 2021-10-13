#include "EnemyManager.h"
#include "Enemy.h"
#include "Tank.h"

HRESULT EnemyManager::Init(TILE_INFO* tileMap)
{
	enemyMaxCount = 6;
	vecEnemys.reserve(enemyMaxCount);
	tileInfo = tileMap;

	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));
	return S_OK;
}

void EnemyManager::Update()
{
	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end(); itEnemys++)
	{
		(*itEnemys)->Update();
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

void EnemyManager::AddEnemy(Tank* tank,POINTFLOAT pos)
{
	tank->Init(tileInfo);
	tank->SetPos(pos);
	vecEnemys.push_back(tank);
}
