#include "Stage.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"

#include "AmmoManager.h"
#include "Tank.h"
#include "TankFactory.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "ScoreScene.h"

HRESULT Stage::Init()
{
	SetWindowSize(20, 20, WIN_SIZE_X, WIN_SIZE_Y);
	ImageManager::GetSingleton()->AddImage("Image/Tile3.bmp",
		128, 32, 8, 2, true, RGB(255, 0, 255));
	sampleImage = ImageManager::GetSingleton()->FindImage("Image/Tile3.bmp");
	if (sampleImage == nullptr)
	{
		cout << "Image/Tile3.bmp 로드 실패!!" << endl;
		return E_FAIL;
	}

	remainMonster = 18;
	remainSpawnMonster = 18;

	spawnMonsterImage = ImageManager::GetSingleton()->FindImage("Image/Icon/Icon_Enemy.bmp");

	lifeImage = ImageManager::GetSingleton()->FindImage("Image/Icon/player1Life.bmp");

	stageImage = ImageManager::GetSingleton()->FindImage("Image/Icon/StageFlag.bmp");

	stageLevel = ImageManager::GetSingleton()->FindImage("Image/Text/Number.bmp");

	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");

	slate = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	slate1 = -(backGround->GetHeight()) + 200;
	slate2 = backGround->GetHeight() + 210;	//닫

	gameOver = ImageManager::GetSingleton()->FindImage("Image/Text/Game_Over.bmp");
	gameOverPosY = WIN_SIZE_Y + 30;

	int nextStage =((GameManager::GetSingleton()->stageLevel - 1) % 3 + 1);
	Load(nextStage);

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
	vecTankFactorial[0] = new PlayerTankFactory;
	vecTankFactorial[1] = new NormalEnemyTankFactory;
	vecTankFactorial[2] = new SpeedEnemyTankFactory;
	vecTankFactorial[3] = new RapidEnemyTankFactory;
	vecTankFactorial[4] = new DefensiveEnemyTankFactory;

	playerTankAmmoManager = new AmmoManager;
	enemyTankAmmoManager = new AmmoManager;

	tank = vecTankFactorial[0]->CreateTank();
	enemyMgr = new EnemyManager;

	itemManager = new ItemManager;

	tank->Init(playerTankAmmoManager, enemyTankAmmoManager, tileInfo, enemyMgr->GetAddresVecEnemys(), tank, itemManager);
	enemyMgr->Init(enemyTankAmmoManager, playerTankAmmoManager, tileInfo, tank, this);
	playerTankAmmoManager->Init(tileInfo, nullptr, enemyMgr->GetAddresVecEnemys());
	enemyTankAmmoManager->Init(tileInfo, tank);

	backGroundRect.left = STAGE_SIZE_X;
	backGroundRect.top = STAGE_SIZE_Y;
	backGroundRect.right = STAGE_SIZE_X + 416;
	backGroundRect.bottom = STAGE_SIZE_Y + 416;

	for (int i = 0; i < 2; i++)
	{
		boomImg[i].BoomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Integrated_Boom_Effect.bmp");
	}

	elapsedCount = 0;


	spawnCount = 0;
	GameManager::GetSingleton()->remainSpawnMonster = 2;
	GameManager::GetSingleton()->remainMonster = 2;

	stateElapsedCount = 0;
	return S_OK;
}

void Stage::Update()
{
	//게임 끝났을 때 
	if (GameManager::GetSingleton()->state == GameState::Done)
	{
		CloseSlate(); // 게임 종료시 슬레이트 닫기

	}
	else if (GameManager::GetSingleton()->state == GameState::Playing || GameManager::GetSingleton()->state == GameState::DestoryNexus) //게임 진행중
	{
		tank->Update();

		//SpawnEnemyTank
		SpawnEnemyTank();
		
		//ShowBulletCollider
		ShowBulletCollider();

		//ShowBodyCollider
		ShowBodyCollider();

		//PlayerTankDestroyAnimation
		PlayerTankDestroyAnimation();

		//NexusDestoryAnimation
		NexusDestoryAnimation();

		//WaterTileAnimation
		WaterTileAnimation();
	
		//RotateToScoreScene
		RotateToScoreScene();

		//RotateToGameOverState
		RotateToGameOverState();
	}
	else if (GameManager::GetSingleton()->state == GameState::GameOver) 
	{
		RotateGameOverScene();
	}

	itemManager->Update();
	playerTankAmmoManager->Update();
	enemyTankAmmoManager->Update();
	enemyMgr->Update();
}

void Stage::Render(HDC hdc)
{
	// 배경
	backGround->Render(hdc);
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

	FillRect(hdc, &backGroundRect, myBrush);

	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);

	// 메인 영역
	TileRender(hdc);

	//RemainEnemyRender
	RemainEnemyRender(hdc);

	tank->Render(hdc);
	enemyMgr->Render(hdc);
	itemManager->Render(hdc);
	playerTankAmmoManager->Render(hdc);
	enemyTankAmmoManager->Render(hdc);

	GrassTileRender(hdc);


	lifeImage->Render(hdc, 480, 260);
	PlayerLifeRender(hdc);


	stageImage->Render(hdc, 480, 370);
	StageLevelRender(hdc);

	PlayerTankDestroyRender(hdc);

	NexusDestroyRender(hdc);


	slate->Render(hdc, backGround->GetWidth() / 2, slate1);
	slate->Render(hdc, backGround->GetWidth() / 2, slate2);


	gameOver->Render(hdc, STAGE_SIZE_X + 208, gameOverPosY);
}

void Stage::Release()
{
}

void Stage::SpawnEnemy(TankType type)
{
	enemyMgr->AddEnemy(vecTankFactorial[(int)type + 1]->CreateTank(), spawnEnemyPos[GameManager::GetSingleton()->spawnCount++]);

	if (GameManager::GetSingleton()->spawnCount >= maxSpawnCount)
	{
		GameManager::GetSingleton()->spawnCount -= maxSpawnCount;
	}

}

void Stage::CreateItem()
{
	for (int i = 0; i < 1; )
	{
		int randtile = rand() % (TILE_COUNT_X * TILE_COUNT_Y);

		if (tileInfo[randtile].tileType == TileType::Ground)
		{
			int itemtype = rand() % 6;
			itemManager->Init(itemtype, randtile, tank, enemyMgr, tileInfo);
			break;
		}
	}
}

void Stage::CloseSlate()
{
	slate1 += 10;
	slate2 -= 10;	//닫

	if (slate1 >= 0)
	{
		GameManager::GetSingleton()->state = GameState::Playing;
		GameManager::GetSingleton()->stageLevel++;

		SceneManager::GetSingleton()->ChangeScene("LoadingScene");

		slate1 = -(backGround->GetHeight()) + 200;
		slate2 = backGround->GetHeight() + 200;	//닫
	}
}

void Stage::RotateGameOverScene()
{
	if (gameOverPosY > WIN_SIZE_Y / 2)
	{
		gameOverPosY -= 2;
	}
	else
	{
		stateElapsedCount++;
		if (stateElapsedCount > 100)
		{
			SceneManager::GetSingleton()->ChangeScene("ScoreScene");
		}
	}
}

void Stage::RotateToGameOverState()
{
	if (GameManager::GetSingleton()->player1Life < 0 || GameManager::GetSingleton()->state == GameState::DestoryNexus)
	{
		stateElapsedCount++;
		if (stateElapsedCount >= 120)
		{
			stateElapsedCount = 0;
			GameManager::GetSingleton()->state = GameState::GameOver;
		}
	}
}

void Stage::RotateToScoreScene()
{
	if (GameManager::GetSingleton()->remainMonster <= 0)
	{
		elapsedCount++;
		if (elapsedCount > 200) {
			elapsedCount = 0;
			GameManager::GetSingleton()->state = GameState::Done;
			GameManager::GetSingleton()->spawnCount = 0;
			GameManager::GetSingleton()->playerEnforceCount = tank->GetEnforceCount();
			SceneManager::GetSingleton()->AddScene("scoreScene", new ScoreScene());
			SceneManager::GetSingleton()->ChangeScene("scoreScene");
		}
	}
}

void Stage::WaterTileAnimation()
{
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

void Stage::NexusDestoryAnimation()
{
	if (tileInfo[636].frameX == 4)
	{
		boomImg[1].bRenderBoomImg = true;
		POINTFLOAT temp = { (tileInfo[636].collider.right), (tileInfo[636].collider.bottom) };
		boomImg[1].imgPos = temp;
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
			}
		}
	}
}

void Stage::ShowBodyCollider()
{
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
}

void Stage::ShowBulletCollider()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('W'))
	{
		if (check)
			check = false;
		else
			check = true;
	}
}

void Stage::SpawnEnemyTank()
{
	elapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
	if (elapsedCount >= spawmElapsedCount && currSpawnEnemy < maxSpawnEnemy && GameManager::GetSingleton()->remainSpawnMonster>0)
	{
		GameManager::GetSingleton()->remainSpawnMonster--;
		currSpawnEnemy++;
		elapsedCount -= spawmElapsedCount;
		int randomType = RANDOM(0, 3);
		SpawnEnemy((TankType)randomType);
	}
}

void Stage::PlayerTankDestroyAnimation()
{
	if (GameManager::GetSingleton()->player1Life >= 0)
	{
		if (!(boomImg[0].bRenderBoomImg) && tank->GetHP() <= 0)
		{
			boomImg[0].bRenderBoomImg = true;
			boomImg[0].imgPos = tank->GetPos();
			delete tank;
			tank = vecTankFactorial[0]->CreateTank();
			for (itEnemyTanks = enemyMgr->GetAddresVecEnemys()->begin(); itEnemyTanks != enemyMgr->GetAddresVecEnemys()->end(); itEnemyTanks++)
			{
				(*itEnemyTanks)->SetPlayerTank(tank);
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
					tank->Init(playerTankAmmoManager, enemyTankAmmoManager, tileInfo, enemyMgr->GetAddresVecEnemys(), tank, itemManager);
			}
		}
	}
}

void Stage::TileRender(HDC hdc)
{
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

}

void Stage::RemainEnemyRender(HDC hdc)
{
	for (int i = 0; i < GameManager::GetSingleton()->remainSpawnMonster; i++)
	{
		if (i % 2 == 0)
			spawnMonsterImage->Render(hdc, 472, 35 + 16 * (i / 2));
		else
			spawnMonsterImage->Render(hdc, 472 + 16, 35 + 16 * (i / 2));
	}
}

void Stage::GrassTileRender(HDC hdc)
{
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
}

void Stage::PlayerLifeRender(HDC hdc)
{
	if (GameManager::GetSingleton()->player1Life >= 0)
	{
		stageLevel->Render(hdc, 494, 270, GameManager::GetSingleton()->player1Life % 5, GameManager::GetSingleton()->player1Life / 5);
	}
	else
	{
		stageLevel->Render(hdc, 494, 270, 0, 0);
	}
}

void Stage::StageLevelRender(HDC hdc)
{
	if (GameManager::GetSingleton()->stageLevel < 10)
	{
		stageLevel->Render(hdc, 490, 390, GameManager::GetSingleton()->stageLevel % 5, GameManager::GetSingleton()->stageLevel / 5);
	}
	else if (GameManager::GetSingleton()->stageLevel >= 10)
	{
		stageLevel->Render(hdc, 490, 390, GameManager::GetSingleton()->stageLevel / 10, GameManager::GetSingleton()->stageLevel / 50);
		stageLevel->Render(hdc, 502, 390, (GameManager::GetSingleton()->stageLevel % 10) % 5, (GameManager::GetSingleton()->stageLevel % 10) / 5);
	}
}

void Stage::PlayerTankDestroyRender(HDC hdc)
{
	if (boomImg[0].bRenderBoomImg)
	{
		boomImg[0].BoomImg->Render(hdc, boomImg[0].imgPos.x - STAGE_SIZE_X / 2, boomImg[0].imgPos.y - STAGE_SIZE_Y, boomImg[0].BoomImgCurrFrame, 0);
	}
}

void Stage::NexusDestroyRender(HDC hdc)
{
	if (boomImg[1].bRenderBoomImg)
	{
		boomImg[1].BoomImg->Render(hdc, boomImg[1].imgPos.x, boomImg[1].imgPos.y, boomImg[1].BoomImgCurrFrame, 0);
	}
}

void Stage::Load(int index)
{
	string filePath = "Save/saveMapData" + to_string(index) + ".map";

	HANDLE hFile = CreateFile(filePath.c_str(),
		GENERIC_READ,                  //읽기, 쓰기 타입
		0, NULL,                        //공유, 보안 모드
		OPEN_EXISTING,                  //파일을 만들거나 읽을 때 옵션
		FILE_ATTRIBUTE_NORMAL,          //파일 속성(읽기 전용, 숨김 등등)
		NULL);                          //

	//읽기

	DWORD readByte;
	if (ReadFile(hFile, tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);
}
