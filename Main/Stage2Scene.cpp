#include "Stage2Scene.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"

#include "Tank.h"
#include "TankFactorial.h"
#include "EnemyManager.h"


#include "Stage3Scene.h"
#include "ScoreScene.h"

HRESULT Stage2Scene::Init()
{
	SetWindowSize(20, 20, WIN_SIZE_X, WIN_SIZE_Y);
	ImageManager::GetSingleton()->AddImage("Image/Tile3.bmp",
		128, 32, 8, 2, true, RGB(255, 0, 255));
	sampleImage = ImageManager::GetSingleton()->FindImage("Image/Tile3.bmp");
	if (sampleImage == nullptr)
	{
		cout << "Image/Tile2.bmp 로드 실패!!" << endl;
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

	slate = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	slate1 = -(backGround->GetHeight()) + 200;
	slate2 = backGround->GetHeight() + 200;	//닫

	Load(2);

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

	tank->Init(tileInfo, enemyMgr, tank);
	enemyMgr->Init(tileInfo, tank, this);


	backGroundRect.left = STAGE_SIZE_X;
	backGroundRect.top = STAGE_SIZE_Y;
	backGroundRect.right = STAGE_SIZE_X + 416;
	backGroundRect.bottom = STAGE_SIZE_Y + 416;


	elapsedCount = 0;


	spawnCount = 0;
	GameManager::GetSingleton()->remainSpawnMonster = 2;
	GameManager::GetSingleton()->remainMonster = 2;


	return S_OK;
}

void Stage2Scene::Update()
{
	if (GameManager::GetSingleton()->state == GameState::Done)
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

void Stage2Scene::Render(HDC hdc)
{
	// 배경
	backGround->Render(hdc);
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

	FillRect(hdc, &backGroundRect, myBrush);

	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);

	// 메인 영역
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
	stageLevel->Render(hdc, 490, 390, 1, 0);

	slate->Render(hdc, backGround->GetWidth() / 2, slate1);
	slate->Render(hdc, backGround->GetWidth() / 2, slate2);
}

void Stage2Scene::Release()
{
}

void Stage2Scene::SpawnEnemy(TankType type)
{
	enemyMgr->AddEnemy(vecTankFactorial[(int)type + 1]->CreateTank(), spawnEnemyPos[GameManager::GetSingleton()->spawnCount++]);

	if (GameManager::GetSingleton()->spawnCount >= maxSpawnCount)
	{
		GameManager::GetSingleton()->spawnCount -= maxSpawnCount;
	}

}

void Stage2Scene::Load(int index)
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
