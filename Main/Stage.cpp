#include "Stage.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"
#include "AmmoManager.h"
#include "Tank.h"
#include "TankFactory.h"
#include "TankManager.h"
#include "ItemManager.h"
#include "ScoreScene.h"

HRESULT Stage::Init()
{

	SetWindowSize(WIN_START_POS_X, WIN_START_POS_Y, WIN_SIZE_X, WIN_SIZE_Y);
	ImageManager::GetSingleton()->AddImage("Image/Tile3.bmp",
		128, 32, 8, 2, true, RGB(255, 0, 255));
	sampleImage = ImageManager::GetSingleton()->FindImage("Image/Tile3.bmp");
	if (sampleImage == nullptr)
	{
		return E_FAIL;
	}

	spawnMonsterImage = ImageManager::GetSingleton()->FindImage("Image/Icon/Icon_Enemy.bmp");
	if (spawnMonsterImage == nullptr)
	{
		return E_FAIL;
	}
	lifeImage = ImageManager::GetSingleton()->FindImage("Image/Icon/player1Life.bmp");
	if (lifeImage == nullptr)
	{
		return E_FAIL;
	}

	stageImage = ImageManager::GetSingleton()->FindImage("Image/Icon/StageFlag.bmp");
	if (stageImage == nullptr)
	{
		return E_FAIL;
	}

	stageLevel = ImageManager::GetSingleton()->FindImage("Image/Text/Number.bmp");
	if (stageLevel == nullptr)
	{
		return E_FAIL;
	}

	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	if (backGround == nullptr)
	{
		return E_FAIL;
	}

	slate = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	if (slate == nullptr)
	{
		return E_FAIL;
	}
	gameOver = ImageManager::GetSingleton()->FindImage("Image/Text/Game_Over.bmp");
	if (gameOver == nullptr)
	{
		return E_FAIL;
	}

	overSlatePos = -(backGround->GetHeight()) + 200;
	lowSlatePos = backGround->GetHeight() + 210;	//닫

	gameOverPosY = WIN_SIZE_Y + 30;

	int nextStage = ((GameManager::GetSingleton()->stageLevel - 1) % 3 + 1);
	Load(nextStage);

	waterTilePos.clear();
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
				waterTilePos.emplace_back(i, j);
				waterTilePos.push_back(make_pair(i, j));
			}
		}
	}

	playerTankAmmoManager = DBG_NEW AmmoManager;
	enemyTankAmmoManager = DBG_NEW AmmoManager;

	tankManager = DBG_NEW TankManager;

	itemManager = DBG_NEW ItemManager;
	itemManager->Init(tileInfo);

	tankManager->Init(enemyTankAmmoManager, playerTankAmmoManager, tileInfo, this, itemManager->GetAddressVecItem());
	playerTankAmmoManager->Init(tileInfo, nullptr, tankManager->GetAddresVecEnemys());
	enemyTankAmmoManager->Init(tileInfo, tankManager->GetPlayerTank());

	backGroundRect.left = STAGE_SIZE_X;
	backGroundRect.top = STAGE_SIZE_Y;
	backGroundRect.right = STAGE_SIZE_X + 416;
	backGroundRect.bottom = STAGE_SIZE_Y + 416;


	boomImg.BoomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Integrated_Boom_Effect.bmp");
	if (boomImg.BoomImg == nullptr)
	{
		return E_FAIL;
	}
	elapsedCount = 0;


	spawnCount = 0;
	GameManager::GetSingleton()->remainSpawnMonster = 3;
	GameManager::GetSingleton()->remainMonster = 3;

	stateElapsedCount = 0;
	return S_OK;
}

void Stage::Update()
{
	cout << GameManager::GetSingleton()->player1GetItemCount << endl;

	//게임 끝났을 때 
	if (GameManager::GetSingleton()->state == GameState::Done)
	{
		if (CloseSlate()) return; // 게임 종료시 슬레이트 닫기

	}
	else if (GameManager::GetSingleton()->state == GameState::Playing || GameManager::GetSingleton()->state == GameState::DestoryNexus) //게임 진행중
	{
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
		if (RotateToScoreScene()) return;

		//RotateToGameOverState
		RotateToGameOverState();
	}
	else if (GameManager::GetSingleton()->state == GameState::GameOver)
	{
		if (RotateGameOverScene()) return;
	}

	itemManager->Update();
	playerTankAmmoManager->Update();
	enemyTankAmmoManager->Update();
	tankManager->Update();
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

	tankManager->Render(hdc);
	itemManager->Render(hdc);
	playerTankAmmoManager->Render(hdc);
	enemyTankAmmoManager->Render(hdc);

	GrassTileRender(hdc);


	lifeImage->Render(hdc, 480, 260);
	PlayerLifeRender(hdc);


	stageImage->Render(hdc, 480, 370);
	StageLevelRender(hdc);

	NexusDestroyRender(hdc);


	slate->Render(hdc, backGround->GetWidth() / 2, overSlatePos);
	slate->Render(hdc, backGround->GetWidth() / 2, lowSlatePos);


	gameOver->Render(hdc, STAGE_SIZE_X + 208, gameOverPosY);
}

void Stage::Release()
{
	SAFE_RELEASE(playerTankAmmoManager);
	SAFE_RELEASE(enemyTankAmmoManager);
	SAFE_RELEASE(tankManager);
	SAFE_RELEASE(itemManager);

	//waterTilePos.clear();

}

void Stage::CreateItem()
{
	int randtile = 0;
	while (true)
	{
		randtile = rand() % (TILE_COUNT_X * TILE_COUNT_Y);
		if (tileInfo[randtile].tileType == TileType::Ground)
		{
			int itemtype = rand() % 6;
			itemManager->CreateItem(itemtype, randtile, tankManager->GetPlayerTank(), tankManager);
			break;
		}
	}
}

bool Stage::CloseSlate()
{
	overSlatePos += 10;
	lowSlatePos -= 10;	//닫

	if (overSlatePos >= 0)
	{
		GameManager::GetSingleton()->state = GameState::Playing;
		GameManager::GetSingleton()->stageLevel++;

		SceneManager::GetSingleton()->ChangeScene("LoadingScene");

		overSlatePos = -(backGround->GetHeight()) + 200;
		lowSlatePos = backGround->GetHeight() + 200;	//닫

		return true;
	}
	return false;
}

bool Stage::RotateGameOverScene()
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
			return true;
		}
	}
	return false;
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

bool Stage::RotateToScoreScene()
{
	if (GameManager::GetSingleton()->remainMonster <= 0)
	{
		elapsedCount++;
		if (elapsedCount > 200) {
			elapsedCount = 0;
			GameManager::GetSingleton()->state = GameState::Done;
			GameManager::GetSingleton()->spawnCount = 0;
			GameManager::GetSingleton()->playerEnforceCount = tankManager->GetPlayerTank()->GetEnforceCount();
			SceneManager::GetSingleton()->AddScene("scoreScene", new ScoreScene());
			SceneManager::GetSingleton()->ChangeScene("scoreScene");
			return true;
		}
	}
	return false;
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
		boomImg.bRenderBoomImg = true;
		POINTFLOAT temp = { (FLOAT)(tileInfo[636].collider.right), (FLOAT)(tileInfo[636].collider.bottom) };
		boomImg.imgPos = temp;
	}


	if (boomImg.bRenderBoomImg)
	{
		boomImg.elapsedCount++;

		if (boomImg.elapsedCount >= boomImg.addImgFrameCount)
		{
			boomImg.elapsedCount = 0;
			boomImg.BoomImgCurrFrame++;

			if (boomImg.BoomImgCurrFrame == boomImg.BoomImgMaxFrame)
			{
				boomImg.bRenderBoomImg = false;
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
		int randomType = RANDOM(3, 3);

		tankManager->AddEnemy((TankType)randomType, spawnEnemyPos[GameManager::GetSingleton()->spawnCount++]);

		if (GameManager::GetSingleton()->spawnCount >= maxSpawnCount)
		{
			GameManager::GetSingleton()->spawnCount -= maxSpawnCount;
		}
	}
}

void Stage::PlayerTankDestroyAnimation()
{

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

void Stage::NexusDestroyRender(HDC hdc)
{
	if (boomImg.bRenderBoomImg)
	{
		boomImg.BoomImg->Render(hdc, (int)(boomImg.imgPos.x), (int)(boomImg.imgPos.y), boomImg.BoomImgCurrFrame, 0);
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
