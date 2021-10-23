#include "Tank.h"
#include "Image.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "AmmoManager.h"
#include "Item.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Stage3Scene.h"

#pragma region PlyaerTank
HRESULT PlayerTank::Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tile, vector<Tank*>* enemyTanks, Tank* playerTank, vector<Item*>* itemList, GameEntity* stageInfo)
{
	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (shieldImg == nullptr) { cout << "ShieldImg nullptr" << endl;  return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }

	pos.x = 200;
	pos.y = 430;

	bodySize = 64;
	moveSpeed = 2.0f;

	this->tileInfo = tile;

	this->ItemList = itemList;

	this->ammoManager = ammoManager;
	this->targetAmmoManager = targetAmmoManager;
	this->enemyTanks = enemyTanks;

	currFireNumberOfAmmo = 0;
	this->stageInfo = stageInfo;
	enforceCount = GameManager::GetSingleton()->playerEnforceCount;

	SetShape();

	if (IsCollided()) { bCheckSpawnCollided = true; }

	moveDir = MoveDir::Up;
	elapsedCount = 0.0f;

	bIsAlive = true;
	bCheckSpawnStatus = true;
	bCheckShieldOn = false;

	barrelPos = { pos.x + bodySize / 2, pos.y + bodySize / 2 };

	bCheckTankCollider = false;

	return S_OK;
}

void PlayerTank::Update()
{
	if (bIsAlive == false)	return;

	if (KeyManager::GetSingleton()->IsOnceKeyDown('P')) { if (enforceCount <= 2) { enforceCount++; } }
	else if (KeyManager::GetSingleton()->IsOnceKeyDown('O')) { if (enforceCount >= 1) { enforceCount--; } }

	SpwanAndShieldAnimation();

	SpawnCollided();

	Action();

	if (KeyManager::GetSingleton()->IsOnceKeyDown('R'))
	{
		if (bCheckTankCollider)
			bCheckTankCollider = false;
		else
			bCheckTankCollider = true;

	}
}

void PlayerTank::Render(HDC hdc)
{
	if (bIsAlive == false)	return;

	if (bCheckTankCollider)
		Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);

	if (bCheckSpawnStatus)
	{
		spawnImg->Render(hdc, (int)(pos.x -bodySize * 0.25f), (int)(pos.y - bodySize * 0.25f), spawnImgFrame, 0, 1.0f);
	}
	else
	{
		img->Render(hdc, (int)(pos.x - bodySize * 0.25f), (int)(pos.y - bodySize * 0.25f), (int)moveDir + checkMoveCount, enforceCount, 1.0f);
	}

	if (bCheckShieldOn)
	{
		shieldImg->Render(hdc, (int)(pos.x - bodySize * 0.25f), (int)(pos.y - bodySize * 0.25f), bShieldImageChanged, 0, 1.0f);
	}
}

void PlayerTank::Release()
{
}

void PlayerTank::Fire()
{
	if (currFireNumberOfAmmo <= 0)
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
		{
			if (currFireNumberOfAmmo <= 0)
				currFireNumberOfAmmo = 0;
			currFireNumberOfAmmo++;

			if (enforceCount == 0) { ammoManager->Fire(this); }
			else { ammoManager->Fire(this, 400.0f); }
		}
	}
	else if (enforceCount > 1 && currFireNumberOfAmmo == 1)
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
		{
			currFireNumberOfAmmo++;
			ammoManager->Fire(this, 400.0f);
		}
	}
}

void PlayerTank::Action()
{
	if (!bCheckSpawnStatus)
	{
		if (Singleton<KeyManager>::GetSingleton()->IsStayKeyDown(VK_LEFT))
		{
			previousDir = moveDir;
			moveDir = MoveDir::Left;
			Move();
		}
		else if (Singleton<KeyManager>::GetSingleton()->IsStayKeyDown(VK_RIGHT))
		{
			previousDir = moveDir;
			moveDir = MoveDir::Right;
			Move();
		}
		else if (Singleton<KeyManager>::GetSingleton()->IsStayKeyDown(VK_UP))
		{
			previousDir = moveDir;
			moveDir = MoveDir::Up;
			Move();
		}
		else if (Singleton<KeyManager>::GetSingleton()->IsStayKeyDown(VK_DOWN))
		{
			previousDir = moveDir;
			moveDir = MoveDir::Down;
			Move();
		}

		Fire();
	}
}

void PlayerTank::SpwanAndShieldAnimation()
{
	elapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
	if (bCheckShieldOn || bCheckSpawnStatus)
	{
		
		spawnElapsedCount += TimerManager::GetSingleton()->GetDeltaTime() * 2;
		if (bCheckSpawnStatus && elapsedCount >= spawnTime) { elapsedCount -= spawnTime; bCheckSpawnStatus = false; bCheckShieldOn = true; }
		if (bCheckShieldOn && elapsedCount >= shieldTime) { bCheckShieldOn = false; }
		if (bCheckSpawnStatus)
		{
			if (spawnElapsedCount > 0.125f)
			{
				spawnElapsedCount -= 0.125f;
				if (bReverseSpawnImg) { spawnImgFrame--; }
				else { spawnImgFrame++; }
				if (spawnImgFrame == 3 || spawnImgFrame == 0)
				{
					bReverseSpawnImg = !bReverseSpawnImg;
				}
			}
		}

		if (bCheckShieldOn)
		{
			shieldElapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
			if (shieldElapsedCount > 0.05f)
			{
				bShieldImageChanged = !bShieldImageChanged;
				shieldElapsedCount -= 0.05f;
			}
		}
	}
}

PlayerTank::PlayerTank()
{
	bIsAlive = false;
	img = ImageManager::GetSingleton()->FindImage("Image/Player/Player.bmp");
	shieldImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Shield.bmp");
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");
}
#pragma endregion

#pragma region NormalEnemyTank
HRESULT NormalEnemyTank::Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tile, vector<Tank*>* enemyTanks, Tank* playerTank, vector<Item*>* itemList, GameEntity* stageInfo)
{
	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");
	itemTank = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy_Item.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }
	if (itemTank == nullptr) { cout << "itemTankImg nullptr" << endl;  return E_FAIL; }

	bodySize = 64;
	moveSpeed = 2.0f;
	type = TankType::Normal;

	this->tileInfo = tile;
	this->playerTank = playerTank;
	this->ammoManager = ammoManager;
	this->stageInfo = stageInfo;
	this->enemyTanks = enemyTanks;

	SetShape();
	if (IsCollided())
	{
		bCheckSpawnCollided = true;
	}

	moveDir = MoveDir::Down;

	bIsAlive = true;

	return S_OK;
}
#pragma endregion

#pragma region SpeedEnemyTank
HRESULT SpeedEnemyTank::Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tile, vector<Tank*>* enemyTanks, Tank* playerTank, vector<Item*>* itemList, GameEntity* stageInfo)
{
	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");
	itemTank = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy_Item.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }
	if (itemTank == nullptr) { cout << "itemTankImg nullptr" << endl;  return E_FAIL; }

	bodySize = 64;
	moveSpeed = 4.0f;
	type = TankType::Speed;

	this->tileInfo = tile;
	this->playerTank = playerTank;
	this->ammoManager = ammoManager;
	this->stageInfo = stageInfo;
	this->enemyTanks = enemyTanks;

	SetShape();
	if (IsCollided()) { bCheckSpawnCollided = true; }

	moveDir = MoveDir::Down;

	bIsAlive = true;

	return S_OK;
}
#pragma endregion

#pragma region RapidEnemyTank
HRESULT RapidEnemyTank::Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tile, vector<Tank*>* enemyTanks, Tank* playerTank, vector<Item*>* itemList, GameEntity* stageInfo)
{
	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");
	itemTank = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy_Item.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }
	if (itemTank == nullptr) { cout << "itemTankImg nullptr" << endl;  return E_FAIL; }

	bodySize = 64;
	moveSpeed = 2.0f;
	type = TankType::Rapid;

	this->tileInfo = tile;
	this->playerTank = playerTank;
	this->ammoManager = ammoManager;
	this->stageInfo = stageInfo;
	this->enemyTanks = enemyTanks;

	SetShape();
	if (IsCollided()) { bCheckSpawnCollided = true; }

	moveDir = MoveDir::Down;

	bIsAlive = true;

	return S_OK;
}
#pragma endregion

#pragma region DefensiveEnemyTank
HRESULT DefensiveEnemyTank::Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tile, vector<Tank*>* enemyTanks, Tank* playerTank, vector<Item*>* itemList, GameEntity* stageInfo)
{
	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");
	itemTank = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy_Item.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }
	if (itemTank == nullptr) { cout << "itemTankImg nullptr" << endl;  return E_FAIL; }

	bodySize = 64;
	moveSpeed = 2.0f;
	HP = 4;
	type = TankType::Defensive;

	this->tileInfo = tile;
	this->playerTank = playerTank;
	this->ammoManager = ammoManager;
	this->stageInfo = stageInfo;
	this->enemyTanks = enemyTanks;

	SetShape();
	if (IsCollided()) { bCheckSpawnCollided = true; }

	moveDir = MoveDir::Down;

	bIsAlive = true;

	return S_OK;
}
#pragma endregion

void Tank::Update()
{
	if (bIsAlive == false)	return;

	elapsedCount += TimerManager::GetSingleton()->GetDeltaTime();

	SpwanAnimation();

	SpawnCollided();

	Action();

	FlashItemTank();


	if (KeyManager::GetSingleton()->IsOnceKeyDown('R'))
	{
		if (bCheckTankCollider)
			bCheckTankCollider = false;
		else
			bCheckTankCollider = true;

	}
}

void Tank::Render(HDC hdc)
{
	if (bIsAlive == false)	return;

		//Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);

	if (bCheckSpawnStatus)
	{
		spawnImg->Render(hdc, (int)(pos.x - bodySize * 0.25f), (int)(pos.y - bodySize * 0.25f), spawnImgFrame, 0, 1.0f);
	}
	else
	{
		if (bHaveItem)
		{
			itemTank->Render(hdc, (int)(pos.x - bodySize * 0.33f), (int)(pos.y - bodySize * 0.33f), (int)moveDir + checkMoveCount, (((int)type - 1) * 2) + checkMoveCount_2, 2.0f);
		}
		else
		{
			img->Render(hdc, (int)pos.x, (int)pos.y, (int)moveDir + checkMoveCount, ((int)type - 1) + (HP / 2), 0.5f);
		}
	}
}

void Tank::Release()
{
}

void Tank::Move()
{
	switch (moveDir)
	{
	case MoveDir::Left:
		CorrectionPosY();

		pos.x -= moveSpeed;
		SetShape();

		if (IsCollided() || shape.left < STAGE_SIZE_X)
		{
			pos.x += moveSpeed;
			SetShape();
		}

		break;
	case MoveDir::Right:
		CorrectionPosY();

		pos.x += moveSpeed;
		SetShape();

		if (IsCollided() || shape.right > 416 + STAGE_SIZE_X)
		{
			pos.x -= moveSpeed;
			SetShape();
		}
		break;
	case MoveDir::Up:
		CorrectionPosX();

		pos.y -= moveSpeed;
		SetShape();

		if (IsCollided() || shape.top < STAGE_SIZE_Y)
		{
			pos.y += moveSpeed;
			SetShape();
		}
		break;
	case MoveDir::Down:
		CorrectionPosX();

		pos.y += moveSpeed;
		SetShape();

		if (IsCollided() || shape.bottom > 416 + STAGE_SIZE_Y)
		{
			pos.y -= moveSpeed;
			SetShape();
		}
		break;
	default:
		break;
	}

	if (type == TankType::Player)
	{
		CheckItem();
	}

	if (checkMoveCount > 0) { checkMoveCount = 0; }
	else { checkMoveCount = 1; }
}

void Tank::Fire()
{
	testelapsed++;

	if (testelapsed >= delay_2)
	{
		testelapsed = 0;
		delay_2 = (float)(RANDOM(10, 15));

		currFireNumberOfAmmo++;
		ammoManager->Fire(this);
	}
}

void Tank::CorrectionPosX()
{
	if (previousDir == MoveDir::Left || previousDir == MoveDir::Right)
	{
		int tempPos = 0;
		if ((int)pos.x % 16 <= CORRECTION_POS_MIN)
		{
			tempPos = (int)pos.x % 16;
			pos.x -= tempPos;
			SetShape();
			if (IsCollided())
			{
				pos.x += tempPos;
				SetShape();
			}
		}
		else if ((int)pos.x % 16 >= CORRECTION_POS_MAX)
		{
			tempPos = 16 - (int)pos.x % 16;
			pos.x += tempPos;
			SetShape();
			if (IsCollided())
			{
				pos.x -= tempPos;
				SetShape();
			}
		}
	}
}

void Tank::CorrectionPosY()
{
	if (previousDir == MoveDir::Up || previousDir == MoveDir::Down)
	{
		int tempPos = 0;
		if ((int)pos.y % 16 <= CORRECTION_POS_MIN)
		{
			tempPos = (int)pos.y % 16;
			pos.y -= tempPos;
			SetShape();
			if (IsCollided())
			{
				pos.y += tempPos;
				SetShape();
			}
		}
		else if ((int)pos.y % 16 >= CORRECTION_POS_MAX)
		{
			tempPos = 16 - (int)pos.y % 16;
			pos.y += tempPos;
			SetShape();
			if (IsCollided())
			{
				pos.y -= tempPos;
				SetShape();
			}
		}
		previousDir = moveDir;
	}
}

bool Tank::IsCollided()
{
	RECT temp = {};

	for (int i = 0; i < TILE_COUNT_Y * TILE_COUNT_X; i++)
	{
		if (IntersectRect(&temp, &tileInfo[i].bodyCollider, &shape))
		{
			return true;
		}
	}

	for (itEnemyTanks = enemyTanks->begin();
		itEnemyTanks != enemyTanks->end(); itEnemyTanks++)
	{
		if ((*itEnemyTanks) == this)
		{
			continue;
		}
		if (IntersectRect(&temp, &((*itEnemyTanks)->shape), &shape))
		{
			if ((*itEnemyTanks)->bCheckSpawnCollided && ((*itEnemyTanks)->moveDir != moveDir || (*itEnemyTanks)->bCheckSpawnStatus))
			{
				return false;
			}
			return true;
		}
	}

	if (playerTank != nullptr)
	{
		if (IntersectRect(&temp, &(playerTank->shape), &shape))
		{

			if (playerTank->bCheckSpawnCollided && playerTank->moveDir != moveDir)
			{
				return false;
			}
			return true;
		}
	}

	return false;
}

void Tank::SetShape()
{
	shape.left = (LONG)(pos.x - (bodySize / 2));
	shape.top = (LONG)(pos.y - (bodySize / 2));
	shape.right = (LONG)(shape.left + bodySize / 2);
	shape.bottom = (LONG)(shape.top + bodySize / 2);
}

void Tank::SpawnCollided()
{
	if (bCheckSpawnCollided)
	{
		if (!IsCollided()) { bCheckSpawnCollided = false; }
	}
}

void Tank::Action()
{
	if (!bCheckSpawnStatus)
	{
		if (!clockItem)
		{
			if (elapsedCount >= delay)
			{
				elapsedCount = 0;
				delay = (float)RANDOM(0, 3);
				previousDir = moveDir;
				moveDir = (MoveDir)(RANDOM(0, 3) * 2);
			}

			Move();

			if (currFireNumberOfAmmo == 0)
			{
				Fire();
			}
			else if (type == TankType::Rapid && currFireNumberOfAmmo == 1)
			{
				Fire();
			}
		}
	}
}

void Tank::SpwanAnimation()
{
	if (bCheckSpawnStatus)
	{
		spawnElapsedCount += TimerManager::GetSingleton()->GetDeltaTime() * 2;
		if (bCheckSpawnStatus && elapsedCount >= spawnTime) { elapsedCount -= spawnTime; bCheckSpawnStatus = false; }

		if (spawnElapsedCount > 0.125f)
		{
			spawnElapsedCount -= 0.125f;
			if (bReverseSpawnImg) { spawnImgFrame--; }
			else { spawnImgFrame++; }
			if (spawnImgFrame == 3 || spawnImgFrame == 0)
			{
				bReverseSpawnImg = !bReverseSpawnImg;
			}
		}
	}
}

void Tank::FlashItemTank()
{
	testelapsed_2++;  //아이템 탱크 깜빡깜빡
	if (testelapsed_2 >= 10)
	{
		testelapsed_2 = 0;
		if (checkMoveCount_2 == 0)
			checkMoveCount_2 = 1;
		else
			checkMoveCount_2 = 0;
	}
}

void Tank::CheckItem()
{
	RECT temp = {};

	for (itItemList = ItemList->begin(); itItemList != ItemList->end(); itItemList++)
	{
		if (IntersectRect(&temp, &(*itItemList)->rc, &shape))
		{
			if (!(*itItemList)->GetUseItem())
			{
				(*itItemList)->UseItem();
				(*itItemList)->SetUseItem(true);
			}
			break;
		}
	}
}