#include "Stage3Scene.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"

#include "Tank.h"
#include "TankFactorial.h"
#include "EnemyManager.h"
#include "ItemManager.h"

#include "ScoreScene.h"

HRESULT Stage3Scene::Init()
{
	SetWindowSize(20, 20, WIN_SIZE_X, WIN_SIZE_Y);
	ImageManager::GetSingleton()->AddImage("Image/Tile3.bmp",
		128, 32, 8, 2, true, RGB(255, 0, 255));
	sampleImage = ImageManager::GetSingleton()->FindImage("Image/Tile3.bmp");
	if (sampleImage == nullptr)
	{
		cout << "Image/Tile2.bmp �ε� ����!!" << endl;
		return E_FAIL;
	}

	remainMonster = 18;
	remainSpawnMonster = 18;

	ImageManager::GetSingleton()->AddImage("Image/Icon/Icon_Enemy.bmp",
		16, 16, 1, 1, true, RGB(255, 0, 255));
	spawnMonsterImage = ImageManager::GetSingleton()->FindImage("Image/Icon/Icon_Enemy.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Icon/player1Life.bmp",
		35, 40, 1, 1, true, RGB(255, 0, 255));
	lifeImage = ImageManager::GetSingleton()->FindImage("Image/Icon/player1Life.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Icon/StageFlag.bmp",
		35, 40, 1, 1, true, RGB(255, 0, 255));
	stageImage = ImageManager::GetSingleton()->FindImage("Image/Icon/StageFlag.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Text/Number.bmp", 60 /*40*/, 28 /*14*/, 5, 2, true, RGB(255, 0, 255));
	stageLevel = ImageManager::GetSingleton()->FindImage("Image/Text/Number.bmp");

	ImageManager::GetSingleton()->AddImage("Image/mapImage.bmp", 1024, 768, 1, 1, true, RGB(255, 0, 255));
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");

	Load(3);

	slate = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	slate1 = -(backGround->GetHeight()) + 200;
	slate2 = backGround->GetHeight() + 210;	//��

	for (int i = 0; i < TILE_COUNT_Y; i++)
	{
		for (int j = 0; j < TILE_COUNT_X; j++)
		{
			tileInfo[i * TILE_COUNT_X + j].collider.left += STAGE_SIZE_X;
			tileInfo[i * TILE_COUNT_X + j].collider.right += STAGE_SIZE_X;
			tileInfo[i * TILE_COUNT_X + j].collider.top += STAGE_SIZE_Y;
			tileInfo[i * TILE_COUNT_X + j].collider.bottom += STAGE_SIZE_Y;


			tileInfo[i * TILE_COUNT_X + j].bodyCollider.left += STAGE_SIZE_X;
			tileInfo[i * TILE_COUNT_X + j].bodyCollider.right += STAGE_SIZE_X;
			tileInfo[i * TILE_COUNT_X + j].bodyCollider.top += STAGE_SIZE_Y;
			tileInfo[i * TILE_COUNT_X + j].bodyCollider.bottom += STAGE_SIZE_Y;

			if (tileInfo[i * TILE_COUNT_X + j].tileType == TileType::Water)
			{
				waterTilePos.push_back(make_pair(i, j));
			}
		}
	}

	spawnEnemyPos[0].x = tileInfo[0].rc.right + STAGE_SIZE_X + 16;
	spawnEnemyPos[0].y = tileInfo[0].rc.bottom + STAGE_SIZE_Y * 2;
	spawnEnemyPos[1].x = tileInfo[12].rc.right + STAGE_SIZE_X + 16;
	spawnEnemyPos[1].y = tileInfo[12].rc.bottom + STAGE_SIZE_Y * 2;
	spawnEnemyPos[2].x = tileInfo[24].rc.right + STAGE_SIZE_X + 16;
	spawnEnemyPos[2].y = tileInfo[24].rc.bottom + STAGE_SIZE_Y * 2;

	vecTankFactorial.resize(5);
	vecTankFactorial[0] = new PlayerTankFactorial;
	vecTankFactorial[1] = new NormalEnemyTankFactorial;
	vecTankFactorial[2] = new SpeedEnemyTankFactorial;
	vecTankFactorial[3] = new RapidEnemyTankFactorial;
	vecTankFactorial[4] = new DefensiveEnemyTankFactorial;

	tank = vecTankFactorial[0]->CreateTank();
	enemyMgr = new EnemyManager;

	itemManager = new ItemManager;

	tank->Init(tileInfo, enemyMgr, tank, itemManager);
	enemyMgr->Init(tileInfo, tank, this);


	backGroundRect.left = STAGE_SIZE_X;
	backGroundRect.top = STAGE_SIZE_Y;
	backGroundRect.right = STAGE_SIZE_X + 416;
	backGroundRect.bottom = STAGE_SIZE_Y + 416;


	elapsedCount = 0;


	spawnCount = 0;
	GameManager::GetSingleton()->remainSpawnMonster = 2;
	GameManager::GetSingleton()->remainMonster =2;


	return S_OK;
}

void Stage3Scene::Update()
{
	if (GameManager::GetSingleton()->state == GameState::Done)
	{
		slate1 += 10;
		slate2 -= 10;	//��

		if (slate1 >= 0)
		{
			GameManager::GetSingleton()->state = GameState::Playing;
			GameManager::GetSingleton()->stageLevel++;

			SceneManager::GetSingleton()->ChangeScene("LoadingScene");

			slate1 = -(backGround->GetHeight()) + 200;
			slate2 = backGround->GetHeight() + 200;	//��
		}

	}
	else
	{
		tank->Update();
		enemyMgr->Update();

		elapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
		if (elapsedCount >= spawmElapsedCount && currSpawnEnemy < maxSpawnEnemy && GameManager::GetSingleton()->remainSpawnMonster>0)
		{
			GameManager::GetSingleton()->remainSpawnMonster--;
			currSpawnEnemy++;
			elapsedCount -= spawmElapsedCount;
			SpawnEnemy(TankType::Normal);
		}

		if (KeyManager::GetSingleton()->IsOnceKeyDown('W'))
		{
			if (check)
				check = false;
			else
				check = true;
		}

		if (KeyManager::GetSingleton()->IsOnceKeyUp('E'))
		{
			if (bShowBodyCollider)
			{
				bShowBodyCollider = false;
			}
			else
			{
				bShowBodyCollider = true;
			}
		}

		if (tank->HP <= 0)
		{
			boomImg[0].bRenderBoomImg = true;
			boomImg[0].imgPos = tank->GetPos();
			delete tank;
			tank = vecTankFactorial[0]->CreateTank();
			tank->Init(tileInfo, enemyMgr, tank, itemManager);
			tank->SetPos({ -50.0f,-50.0f });
		}

		if (tileInfo[636].frameX == 4)
		{
			boomImg[1].bRenderBoomImg = true;
			POINTFLOAT temp = { (tileInfo[636].collider.right), (tileInfo[636].collider.bottom) };
			boomImg[1].imgPos = temp;
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
					tank->Init(tileInfo, enemyMgr, tank, itemManager);
				}
			}
		}

		if (boomImg[1].bRenderBoomImg)
		{
			boomImg[1].elapsedCount++;

			if (boomImg[1].elapsedCount >= boomImg[1].addImgFrameCount)
			{
				boomImg[1].elapsedCount = 0;
				boomImg[1].BoomImgCurrFrame++;

				if (boomImg[1].BoomImgCurrFrame == boomImg[1].BoomImgMaxFrame)
				{
					boomImg[1].bRenderBoomImg = false;
					boomImg[1].BoomImgCurrFrame = 0;

					waterElapsedCount++;
					if (waterElapsedCount == 50)
					{
						for (int i = 0; i < waterTilePos.size(); i++)
						{
							if (tileInfo[waterTilePos[i].first * TILE_COUNT_X + waterTilePos[i].second].frameX + 1 == 7)
								tileInfo[waterTilePos[i].first * TILE_COUNT_X + waterTilePos[i].second].frameX = 3;
							(tileInfo[waterTilePos[i].first * TILE_COUNT_X + waterTilePos[i].second].frameX) += 1;
						}
						waterElapsedCount = 0;
					}

				}
			}
		}

		if (GameManager::GetSingleton()->remainMonster <= 0)
		{
			elapsedCount++;
			if (elapsedCount > 200) {
				elapsedCount = 0;
				GameManager::GetSingleton()->state = GameState::Done;
				GameManager::GetSingleton()->spawnCount = 0;
				SceneManager::GetSingleton()->AddScene("scoreScene", new ScoreScene());
				SceneManager::GetSingleton()->ChangeScene("scoreScene");
			}
		}
	}

}

void Stage3Scene::Render(HDC hdc)
{
	// ���
	backGround->Render(hdc);
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

	FillRect(hdc, &backGroundRect, myBrush);

	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);

	// ���� ����
	for (int i = 0; i < TILE_COUNT_Y; i++)
	{
		for (int j = 0; j < TILE_COUNT_X; j++)
		{
			sampleImage->Render(hdc,
				tileInfo[i * TILE_COUNT_X + j].rc.left + TILE_SIZE / 2 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16,
				tileInfo[i * TILE_COUNT_X + j].rc.top + TILE_SIZE / 2 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y,
				tileInfo[i * TILE_COUNT_X + j].frameX,
				tileInfo[i * TILE_COUNT_X + j].frameY, tileInfo[i * TILE_COUNT_X + j].leftHit, tileInfo[i * TILE_COUNT_X + j].rightHit,
				tileInfo[i * TILE_COUNT_X + j].topHit, tileInfo[i * TILE_COUNT_X + j].bottomHit);

			if (check)
			{
				Rectangle(hdc, tileInfo[i * TILE_COUNT_X + j].collider.left,
					tileInfo[i * TILE_COUNT_X + j].collider.top,
					tileInfo[i * TILE_COUNT_X + j].collider.right,
					tileInfo[i * TILE_COUNT_X + j].collider.bottom);
			}
			if (bShowBodyCollider)
			{
				Rectangle(hdc, tileInfo[i * TILE_COUNT_X + j].bodyCollider.left,
					tileInfo[i * TILE_COUNT_X + j].bodyCollider.top,
					tileInfo[i * TILE_COUNT_X + j].bodyCollider.right,
					tileInfo[i * TILE_COUNT_X + j].bodyCollider.bottom);
			}
		}

	}

	for (int i = 0; i < GameManager::GetSingleton()->remainSpawnMonster; i++)
	{
		if (i % 2 == 0)
			spawnMonsterImage->Render(hdc, 472, 35 + 16 * (i / 2));
		else
			spawnMonsterImage->Render(hdc, 472 + 16, 35 + 16 * (i / 2));
	}

	tank->Render(hdc);
	enemyMgr->Render(hdc);

	for (int i = 0; i < TILE_COUNT_Y; i++)
	{
		for (int j = 0; j < TILE_COUNT_X; j++)
		{
			if (tileInfo[i * TILE_COUNT_X + j].tileType == TileType::Grass)
			{
				sampleImage->Render(hdc,
					tileInfo[i * TILE_COUNT_X + j].rc.left + TILE_SIZE / 2 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16,
					tileInfo[i * TILE_COUNT_X + j].rc.top + TILE_SIZE / 2 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y,
					tileInfo[i * TILE_COUNT_X + j].frameX,
					tileInfo[i * TILE_COUNT_X + j].frameY, tileInfo[i * TILE_COUNT_X + j].leftHit, tileInfo[i * TILE_COUNT_X + j].rightHit,
					tileInfo[i * TILE_COUNT_X + j].topHit, tileInfo[i * TILE_COUNT_X + j].bottomHit);
			}
		}
	}


	lifeImage->Render(hdc, 480, 260);
	stageImage->Render(hdc, 480, 370);

	if (GameManager::GetSingleton()->stageLevel < 10)
	{
		stageLevel->Render(hdc, 490, 390, GameManager::GetSingleton()->stageLevel % 5, GameManager::GetSingleton()->stageLevel / 5);
	}
	else if (GameManager::GetSingleton()->stageLevel >= 10)
	{
		stageLevel->Render(hdc, 490, 390, GameManager::GetSingleton()->stageLevel / 10, GameManager::GetSingleton()->stageLevel / 50);
		stageLevel->Render(hdc, 502, 390, (GameManager::GetSingleton()->stageLevel % 10) % 5, (GameManager::GetSingleton()->stageLevel % 10) / 5);
	}


	slate->Render(hdc, backGround->GetWidth() / 2, slate1);
	slate->Render(hdc, backGround->GetWidth() / 2, slate2);

}

void Stage3Scene::Release()
{
}

void Stage3Scene::SpawnEnemy(TankType type)
{
	enemyMgr->AddEnemy(vecTankFactorial[(int)type + 1]->CreateTank(), spawnEnemyPos[GameManager::GetSingleton()->spawnCount++]);


	if (GameManager::GetSingleton()->spawnCount >= maxSpawnCount)
	{
		GameManager::GetSingleton()->spawnCount -= maxSpawnCount;
	}

}

void Stage3Scene::CreateItem()
{
	for (int i = 0; i < 1; )
	{
		int randtile = rand() % (TILE_COUNT_X * TILE_COUNT_Y);

		if (tileInfo[randtile].tileType == TileType::Ground)
		{
			int itemtype = 3;//rand() % 7;
			//itemManager->Init(itemtype, randtile);
			cout << "EnemyManager :" << randtile << "    " << itemtype << endl;
			break;
		}
	}
}

void Stage3Scene::Load(int index)
{
	string filePath = "Save/saveMapData" + to_string(index) + ".map";

	HANDLE hFile = CreateFile(filePath.c_str(),
		GENERIC_READ,                  //�б�, ���� Ÿ��
		0, NULL,                        //����, ���� ���
		OPEN_EXISTING,                  //������ ����ų� ���� �� �ɼ�
		FILE_ATTRIBUTE_NORMAL,          //���� �Ӽ�(�б� ����, ���� ���)
		NULL);                          //

	//�б�

	DWORD readByte;
	if (ReadFile(hFile, tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "�� ������ �ε忡 �����߽��ϴ�.", "����", MB_OK);
	}

	CloseHandle(hFile);
}
