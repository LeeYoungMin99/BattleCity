#include "EnemyManager.h"
#include "Enemy.h"
#include "Tank.h"
#include "Image.h"
#include "Stage1Scene.h"
#include "ItemManager.h"

HRESULT EnemyManager::Init(TILE_INFO* tile, Tank* playerTank, GameEntity* stageInfo)
{
	enemyMaxCount = 6;
	vecEnemys.reserve(enemyMaxCount);

	tileInfo = tile;
	this->playerTank = playerTank;

	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Effect/Big_Boom_Effect.bmp", 128, 64, 2, 1, true, RGB(255, 0, 255));
	for (int i = 0; i < 3; i++)
	{
		boomImg[i].bigBoomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Big_Boom_Effect.bmp");
		boomImg[i].smallBoomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Boom_Effect.bmp");
	}

	this->tileInfo = tile;
	this->stageInfo = stageInfo;
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
				if (boomImg[i].bRenderSmallBoomImg == false)
				{
					boomImg[i].bRenderSmallBoomImg = true;
					boomImg[i].imgPos = (*itEnemys)->GetPos();
					boomImg[i].smallBoomImgCurrFrame = 0;
					boomImg[i].bigBoomImgCurrFrame = 0;
					break;
				}
			}
			if ((*itEnemys)->bItem)
			{
				((Stage1Scene*)stageInfo)->CreateItem();
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
		if (boomImg[i].bRenderSmallBoomImg)
		{
			boomImg[i].elapsedCount++;

			if (boomImg[i].elapsedCount > boomImg[i].addImgFrameCount)
			{
				boomImg[i].smallBoomImgCurrFrame++;
				boomImg[i].elapsedCount = 0;

				if (boomImg[i].smallBoomImgCurrFrame == boomImg[i].smallBoomImgMaxFrame)
				{
					boomImg[i].bRenderSmallBoomImg = false;
					boomImg[i].bRenderBigBoomImg = true;
				}
			}
		}
		else if (boomImg[i].bRenderBigBoomImg)
		{
			boomImg[i].elapsedCount++;
			if (boomImg[i].elapsedCount > boomImg[i].addImgFrameCount)
			{
				boomImg[i].bigBoomImgCurrFrame++;
				boomImg[i].elapsedCount = 0;

				if (boomImg[i].bigBoomImgCurrFrame == boomImg[i].bigBoomImgMaxFrame)
				{
					boomImg[i].bRenderBigBoomImg = false;
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
		if (boomImg[i].bRenderSmallBoomImg)
		{
			boomImg[i].smallBoomImg->Render(hdc, boomImg[i].imgPos.x - STAGE_SIZE_X / 2, boomImg[i].imgPos.y - STAGE_SIZE_Y, boomImg[i].smallBoomImgCurrFrame, 0);
		}
		else if (boomImg[i].bRenderBigBoomImg)
		{
			boomImg[i].bigBoomImg->Render(hdc, boomImg[i].imgPos.x - STAGE_SIZE_X / 2, boomImg[i].imgPos.y - STAGE_SIZE_Y, boomImg[i].bigBoomImgCurrFrame, 0);
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

	tank->Init(tileInfo, this, playerTank);
	elapsedcount++;
	if (elapsedcount%2  == 0/*RANDOM_2(0, 16)*/)
	{
		tank->bItem = true;
	}
	vecEnemys.push_back(tank);
}
