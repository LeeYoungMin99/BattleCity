#include "EnemyManager.h"
#include "Enemy.h"
#include "Tank.h"
#include "Image.h"
#include "Stage.h"
#include "ItemManager.h"


HRESULT EnemyManager::Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tile, Tank* playerTank, GameEntity* stageInfo)
{
	enemyMaxCount = 6;
	vecEnemys.reserve(enemyMaxCount);

	tileInfo = tile;
	this->playerTank = playerTank;

	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Effect/Integrated_Boom_Effect.bmp", 320, 64, 5, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Icon/Point.bmp", 120, 24, 5, 1, true, RGB(255, 0, 255));
	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{
		boomImg[i].BoomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Integrated_Boom_Effect.bmp");
		scoreImg[i].scoreImg = ImageManager::GetSingleton()->FindImage("Image/Icon/Point.bmp");
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

		if ((*itEnemys)->GetHP() <= 0)
		{
			for (int i = 0; i < NUMBER_OF_IMAGES; i++)
			{
				if (boomImg[i].bRenderBoomImg == false)
				{
					boomImg[i].bRenderBoomImg = true;
					boomImg[i].imgPos = (*itEnemys)->GetPos();
					boomImg[i].BoomImgCurrFrame = 0;
					scoreImg[i].tankHP = (*itEnemys)->GetHP();
					scoreImg[i].imgPos = (*itEnemys)->GetPos();
					scoreImg[i].scoreFrame = (int)((*itEnemys)->GetType());
					break;
				}
			}
			if ((*itEnemys)->GetHaveItem())
			{
				((Stage*)stageInfo)->CreateItem();
			}

			Tank* temp = (*itEnemys);
			itEnemys = vecEnemys.erase(itEnemys);
			delete temp;
			((Stage*)stageInfo)->SubCurrSpawnEnemy();
		}
		else
		{
			itEnemys++;
		}
	}

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
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
					if (scoreImg[i].tankHP == 0)
					{
						scoreImg[i].bRenderScore = true;
					}
					else
					{
						boomImg[i].bRenderBoomImg = false;
					}
				}
			}
		}

		if (scoreImg[i].bRenderScore)
		{
			scoreImg[i].elapsedCount++;

			if (scoreImg[i].elapsedCount > scoreImg[i].maxElapsedCount)
			{
				scoreImg[i].elapsedCount = 0;
				scoreImg[i].bRenderScore = false;
				boomImg[i].bRenderBoomImg = false;
			}
		}
	}



	if (clockItem)
	{
		for (itEnemys = vecEnemys.begin();
			itEnemys != vecEnemys.end(); itEnemys++)
		{
			(*itEnemys)->SetClockItem(true);
		}

		elapsedcount_2++;
		if (elapsedcount_2 >= 200)
		{
			elapsedcount_2 = 0;
			for (itEnemys = vecEnemys.begin();
				itEnemys != vecEnemys.end(); itEnemys++)
			{
				(*itEnemys)->SetClockItem(false);
			}
			clockItem = false;
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

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{
		if (boomImg[i].bRenderBoomImg)
		{
			boomImg[i].BoomImg->Render(hdc, boomImg[i].imgPos.x - STAGE_SIZE_X / 2, boomImg[i].imgPos.y - STAGE_SIZE_Y, boomImg[i].BoomImgCurrFrame, 0);
		}

		if (scoreImg[i].bRenderScore)
		{
			scoreImg[i].scoreImg->Render(hdc, scoreImg[i].imgPos.x - STAGE_SIZE_X / 2, scoreImg[i].imgPos.y - STAGE_SIZE_Y, scoreImg[i].scoreFrame, 0);
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

	tank->Init(ammoManager, targetAmmoManager, tileInfo, &vecEnemys, playerTank);

	int a = 0, b = RANDOM_2(0, 3);
	if (a == b)
	{
		tank->SetHaveItem(true);
	}
	vecEnemys.push_back(tank);
}

void EnemyManager::BoomItem()
{
	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end(); itEnemys++)
	{
		GameManager::GetSingleton()->remainMonster--;
		(*itEnemys)->SubtractHP(5);
	}
}

