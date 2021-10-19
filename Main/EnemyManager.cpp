#include "EnemyManager.h"
#include "Enemy.h"
#include "Tank.h"
#include "Image.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "ItemManager.h"


HRESULT EnemyManager::Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tile, Tank* playerTank, GameEntity* stageInfo)
{
	enemyMaxCount = 6;
	vecEnemys.reserve(enemyMaxCount);

	tileInfo = tile;
	this->playerTank = playerTank;

	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Effect/Integrated_Boom_Effect.bmp", 320, 64, 5, 1, true, RGB(255, 0, 255));
	for (int i = 0; i < 3; i++)
	{
		boomImg[i].BoomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Integrated_Boom_Effect.bmp");
	}

	this->tileInfo = tile;
	this->stageInfo = stageInfo;
	this->ammoManager = ammoManager;
	this->targetAmmoManager = targetAmmoManager;

	itemManager = new ItemManager;

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
			for (int i = 0; i < 3; i++)
			{
				if (boomImg[i].bRenderBoomImg == false)
				{
					boomImg[i].bRenderBoomImg = true;
					boomImg[i].imgPos = (*itEnemys)->GetPos();
					boomImg[i].BoomImgCurrFrame = 0;
					break;
				}
			}
			if ((*itEnemys)->bItem)
			{
				int i = ((GameManager::GetSingleton()->stageLevel - 1) % 3 + 1);
				cout << i << endl;
				switch (i)
				{
				case 1:
					((Stage1Scene*)stageInfo)->CreateItem();
					break;
				case 2:
					((Stage2Scene*)stageInfo)->CreateItem();
					break;
				case 3:
					((Stage3Scene*)stageInfo)->CreateItem();
					break;
				}
			}

			Tank* temp = (*itEnemys);
			itEnemys = vecEnemys.erase(itEnemys);
			delete temp;
			((Stage1Scene*)stageInfo)->SubCurrSpawnEnemy();
		}
		else
		{
			itEnemys++;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (boomImg[i].bRenderBoomImg)
		{
			boomImg[i].elapsedCount++;

			if (boomImg[i].elapsedCount > boomImg[i].addImgFrameCount)
			{
				boomImg[i].BoomImgCurrFrame++;
				boomImg[i].elapsedCount = 0;

				if (boomImg[i].BoomImgCurrFrame == boomImg[i].BoomImgMaxFrame)
				{
					boomImg[i].bRenderBoomImg = false;
				}
			}
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

	for (int i = 0; i < 3; i++)
	{
		if (boomImg[i].bRenderBoomImg)
		{
			boomImg[i].BoomImg->Render(hdc, boomImg[i].imgPos.x - STAGE_SIZE_X / 2, boomImg[i].imgPos.y - STAGE_SIZE_Y, boomImg[i].BoomImgCurrFrame, 0);
		}
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

	tank->Init(ammoManager, targetAmmoManager, tileInfo, this, playerTank);
	elapsedcount++;
	if (elapsedcount >= 0/*RANDOM_2(0, 16)*/)
	{
		tank->bItem = true;
	}
	vecEnemys.push_back(tank);
}
