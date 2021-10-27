#include "TankManager.h"
#include "Tank.h"
#include "Image.h"
#include "Stage.h"
#include "TankFactory.h"
#include "ItemManager.h"
#include "Item.h"

HRESULT TankManager::Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tile, GameEntity* stageInfo, vector<Item*>* vecItems)
{
	enemyMaxCount = 6;
	vecEnemys.reserve(enemyMaxCount);

	tileInfo = tile;

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{
		boomImg[i].BoomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Integrated_Boom_Effect.bmp");
		scoreImg[i].scoreImg = ImageManager::GetSingleton()->FindImage("Image/Icon/Point.bmp");
	}

	tankFactory[0] = new PlayerTankFactory;
	tankFactory[1] = new NormalEnemyTankFactory;
	tankFactory[2] = new SpeedEnemyTankFactory;
	tankFactory[3] = new RapidEnemyTankFactory;
	tankFactory[4] = new DefensiveEnemyTankFactory;

	this->playerTank = tankFactory[0]->CreateTank();
	this->tileInfo = tile;
	this->stageInfo = stageInfo;
	this->ammoManager = ammoManager;
	this->targetAmmoManager = targetAmmoManager;
	this->addressVecItems = vecItems;

	this->playerTank->Init(targetAmmoManager, ammoManager, tileInfo, &vecEnemys, nullptr, addressVecItems);

	return S_OK;
}

void TankManager::Update()
{
	playerTank->Update();

	PlayerTankDestroyAnimation();

	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end();)
	{
		(*itEnemys)->Update();

		if ((*itEnemys)->GetHP() <= 0)
		{
			for (int i = 1; i < NUMBER_OF_IMAGES; i++)
			{
				if (boomImg[i].bRenderBoomImg == false)
				{
					boomImg[i].bRenderBoomImg = true;
					boomImg[i].imgPos = (*itEnemys)->GetPos();
					boomImg[i].BoomImgCurrFrame = 0;
					scoreImg[i].tankHP = (*itEnemys)->GetHP();
					scoreImg[i].imgPos = (*itEnemys)->GetPos();
					scoreImg[i].scoreFrame = ((int)((*itEnemys)->GetType())) - 1;
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

	for (int i = 1; i < NUMBER_OF_IMAGES; i++)
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

		fireElapsedCount++;
		if (fireElapsedCount >= 200)
		{
			fireElapsedCount = 0;
			for (itEnemys = vecEnemys.begin();
				itEnemys != vecEnemys.end(); itEnemys++)
			{
				(*itEnemys)->SetClockItem(false);
			}
			clockItem = false;
		}
	}

}


void TankManager::Render(HDC hdc)
{
	playerTank->Render(hdc);

	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end(); itEnemys++)
	{
		(*itEnemys)->Render(hdc);
	}

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{
		if (boomImg[i].bRenderBoomImg)
		{
			boomImg[i].BoomImg->Render(hdc, (int)(boomImg[i].imgPos.x - STAGE_SIZE_X / 2), (int)(boomImg[i].imgPos.y - STAGE_SIZE_Y), boomImg[i].BoomImgCurrFrame, 0);
		}

		if (scoreImg[i].bRenderScore)
		{
			scoreImg[i].scoreImg->Render(hdc, (int)(scoreImg[i].imgPos.x - STAGE_SIZE_X / 2), (int)(scoreImg[i].imgPos.y - STAGE_SIZE_Y), scoreImg[i].scoreFrame, 0);
		}
	}
}

void TankManager::Release()
{
	SAFE_RELEASE(playerTank);

	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(tankFactory[i]);
	}

	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end(); itEnemys++)
	{
		SAFE_RELEASE((*itEnemys));
	}
	vecEnemys.clear();
}

void TankManager::AddEnemy(TankType type, POINTFLOAT pos)
{
	Tank* enemyTank = tankFactory[(int)type]->CreateTank();
	enemyTank->SetPos(pos);

	enemyTank->Init(ammoManager, targetAmmoManager, tileInfo, &vecEnemys, playerTank);

	int a = 0, b = RANDOM(0, 3);
	if (a == b)
	{
		enemyTank->SetHaveItem(true);
	}
	vecEnemys.push_back(enemyTank);
}

void TankManager::BoomItem()
{
	for (itEnemys = vecEnemys.begin();
		itEnemys != vecEnemys.end(); itEnemys++)
	{
		GameManager::GetSingleton()->remainMonster--;
		(*itEnemys)->SubtractHP(5);
	}
}

void TankManager::PlayerTankDestroyAnimation()
{
	if (GameManager::GetSingleton()->player1Life >= 0)
	{
		if (!(boomImg[0].bRenderBoomImg) && playerTank->GetHP() <= 0)
		{
			boomImg[0].bRenderBoomImg = true;
			boomImg[0].imgPos = playerTank->GetPos();
			SAFE_DELETE(playerTank);
			playerTank = tankFactory[0]->CreateTank();
			for (auto iter = vecEnemys.begin();
				iter != vecEnemys.end();
				++iter)
			{
				(*iter)->SetPlayerTank(playerTank);
			}
			GameManager::GetSingleton()->player1Life--;
		}
	}

	if (boomImg[0].bRenderBoomImg)
	{
		boomImg[0].elapsedCount++;

		if (boomImg[0].elapsedCount >= boomImg[0].addImgFrameCount)
		{
			boomImg[0].elapsedCount = 0;
			boomImg[0].BoomImgCurrFrame++;

			if (boomImg[0].BoomImgCurrFrame == boomImg[0].BoomImgMaxFrame)
			{
				boomImg[0].bRenderBoomImg = false;
				boomImg[0].BoomImgCurrFrame = 0;
				if (GameManager::GetSingleton()->player1Life >= 0)
					playerTank->Init(targetAmmoManager, ammoManager, tileInfo, &vecEnemys, nullptr, addressVecItems);
			}
		}
	}
}

