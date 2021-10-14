#include "Tank.h"
#include "Image.h"

#pragma region PlyaerTank
HRESULT PlayerTank::Init(TILE_INFO* tile)
{
	ImageManager::GetSingleton()->AddImage("Image/Player/Player.bmp", 512, 256, 8, 4, true, RGB(255, 0, 255));
	img = ImageManager::GetSingleton()->FindImage("Image/Player/Player.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Effect/Shield.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	shieldImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Shield.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (shieldImg == nullptr) { cout << "ShieldImg nullptr" << endl;  return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }

	pos.x = 200;
	pos.y = 430;

	bodySize = 64;
	moveSpeed = 2.0f;

	tileInfo = tile;

	SetShape();

	moveDir = MoveDir::Up;
	elapsedCount = 0.0f;

	bIsAlive = true;

	ammoCount = 1;
	ammoPack = new Ammo[ammoCount];
	// �̻��� �ʱ�ȭ
	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Init();
	}

	BarrelPos = { pos.x + bodySize / 2, pos.y + bodySize / 2 };

	return S_OK;
}

void PlayerTank::Update()
{
	if (bIsAlive == false)	return;
	ammoPack->Update();

	elapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
	if (bCheckShieldOn || bCheckSpawnStatus)
	{
		// Ÿ�̸Ӱ� 2�ʰ� �Ǹ� ������ ���� ����, ����ð� �ʱ�ȭ
		// Ÿ�̸Ӱ� 3�ʰ� �Ǹ� ���� ����
		spawnElapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
		if (bCheckSpawnStatus && elapsedCount >= spawnTime) { elapsedCount -= spawnTime; bCheckSpawnStatus = false; bCheckShieldOn = true; }
		if (bCheckShieldOn && elapsedCount >= shieldTime) { bCheckShieldOn = false; }
		// Ÿ�̸Ӱ� 0.05�� �������� ���� �̹��� ����
		if (bCheckSpawnStatus)
		{
			spawnElapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
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

	SetShape();
	if (!bCheckSpawnStatus)
	{
		Move();
		Fire();
	}
}

void PlayerTank::Render(HDC hdc)
{
	if (bIsAlive == false)	return;

	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Render(hdc);
	}

	//Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);

	if (bCheckSpawnStatus)
	{
		spawnImg->Render(hdc, pos.x - bodySize * 0.25f, pos.y - bodySize * 0.25f, spawnImgFrame, 0, 1.0f);
	}
	else
	{
		img->Render(hdc, pos.x - bodySize * 0.25f, pos.y - bodySize * 0.25f, moveDir + checkMoveCount, enforceCount, 1.0f);
	}


	// ���� ���� 
	// Ÿ�̸Ӱ� 3�ʰ� �Ǹ� ���� ���� X
	if (bCheckShieldOn)
	{
		shieldImg->Render(hdc, pos.x - bodySize * 0.25f, pos.y - bodySize * 0.25f, bShieldImageChanged, 0, 1.0f);
	}
}

void PlayerTank::Release()
{
	delete[] ammoPack;
}

void PlayerTank::Move()
{
	if (Singleton<KeyManager>::GetSingleton()->IsStayKeyDown(VK_LEFT))
	{
		if (moveDir == MoveDir::Up || moveDir == MoveDir::Down)
		{
			if ((int)pos.y % 16 <= CORRECTION_POS_MIN)
			{
				pos.y -= (int)pos.y % 16;
			}
			else if ((int)pos.y % 16 >= CORRECTION_POS_MAX)
			{
				pos.y += 16 - (int)pos.y % 16;
			}
		}
		moveDir = MoveDir::Left;

		pos.x -= moveSpeed;
		SetShape();
		if (IsCollided() || shape.left < STAGE_SIZE_X)
		{
			pos.x += moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }
	}
	else if (Singleton<KeyManager>::GetSingleton()->IsStayKeyDown(VK_RIGHT))
	{
		if (moveDir == MoveDir::Up || moveDir == MoveDir::Down)
		{
			if ((int)pos.y % 16 <= CORRECTION_POS_MIN)
			{
				pos.y -= (int)pos.y % 16;
			}
			else if ((int)pos.y % 16 >= CORRECTION_POS_MAX)
			{
				pos.y += 16 - (int)pos.y % 16;
			}
		}
		moveDir = MoveDir::Right;

		pos.x += moveSpeed;
		SetShape();

		if (IsCollided() || shape.right > 416 + STAGE_SIZE_X)
		{
			pos.x -= moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }
	}
	else if (Singleton<KeyManager>::GetSingleton()->IsStayKeyDown(VK_UP))
	{
		if (moveDir == MoveDir::Left || moveDir == MoveDir::Right)
		{
			if ((int)pos.x % 16 <= CORRECTION_POS_MIN)
			{
				pos.x -= (int)pos.x % 16;
			}
			else if ((int)pos.x % 16 >= CORRECTION_POS_MAX)
			{
				pos.x += 16 - (int)pos.x % 16;
			}
		}
		moveDir = MoveDir::Up;

		pos.y -= moveSpeed;
		SetShape();
		if (IsCollided() || shape.top < STAGE_SIZE_Y)
		{
			pos.y += moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }
	}
	else if (Singleton<KeyManager>::GetSingleton()->IsStayKeyDown(VK_DOWN))
	{
		if (moveDir == MoveDir::Left || moveDir == MoveDir::Right)
		{
			if ((int)pos.x % 16 <= CORRECTION_POS_MIN)
			{
				pos.x -= (int)pos.x % 16;
			}
			else if ((int)pos.x % 16 >= CORRECTION_POS_MAX)
			{
				pos.x += 16 - (int)pos.x % 16;
			}
		}
		moveDir = MoveDir::Down;

		pos.y += moveSpeed;
		SetShape();
		if (IsCollided() || shape.bottom > 416 + STAGE_SIZE_Y)
		{
			pos.y -= moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }
	}
}

void PlayerTank::Fire()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
	{
		for (int i = 0; i < ammoCount; i++)
		{
			// ��ü �̻����� ��ȸ�ϸ鼭 �߻� �ƴ��� �ȵƴ��� �Ǵ�
			if (ammoPack[i].GetIsFire()/* && ammoPack[i].GetIsAlive()*/)
				continue;

			switch (moveDir)
			{
			case Left:
				BarrelPos = { pos.x - bodySize / 2, pos.y - bodySize / 4 };
				ammoPack[i].SetMoveDir("Left");
				break;
			case Right:
				BarrelPos = { pos.x, pos.y - bodySize / 4 };
				ammoPack[i].SetMoveDir("Right");
				break;
			case Up:
				BarrelPos = { pos.x - bodySize / 4, pos.y - bodySize / 2 };
				ammoPack[i].SetMoveDir("Up");
				break;
			case Down:
				BarrelPos = { pos.x - bodySize / 4, pos.y };
				ammoPack[i].SetMoveDir("Down");
				break;
			default:
				break;
			}

			//ammoPack[i].SetIsAlive(true);
			ammoPack[i].SetPos(BarrelPos);	// �̻��� ��ġ ����
			ammoPack[i].SetIsFire(true);	// �̻��� ���� ����

			break;
		}
	}
}
#pragma endregion

#pragma region NormalEnemyTank
HRESULT NormalEnemyTank::Init(TILE_INFO* tile)
{
	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 256, 8, 4, true, RGB(255, 0, 255));
	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }

	pos.x = TILEMAPTOOL_SIZE_X / 2.0f;
	pos.y = TILEMAPTOOL_SIZE_Y / 2.0f;

	bodySize = 64;
	moveSpeed = 2.0f;

	tileInfo = tile;

	SetShape();

	moveDir = MoveDir::Down;

	bIsAlive = true;

	ammoCount = 30;
	ammoPack = new Ammo[ammoCount];
	// �̻��� �ʱ�ȭ
	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Init();
	}

	return S_OK;
}
#pragma endregion

#pragma region SpeedEnemyTank
HRESULT SpeedEnemyTank::Init(TILE_INFO* tile)
{
	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 256, 8, 4, true, RGB(255, 0, 255));
	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }

	pos.x = TILEMAPTOOL_SIZE_X / 2.0f;
	pos.y = TILEMAPTOOL_SIZE_Y / 2.0f;

	bodySize = 64;
	moveSpeed = 2.0f;

	tileInfo = tile;

	SetShape();

	moveDir = MoveDir::Down;

	bIsAlive = true;

	ammoCount = 30;
	ammoPack = new Ammo[ammoCount];
	// �̻��� �ʱ�ȭ
	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Init();
	}

	return S_OK;
}
#pragma endregion

#pragma region RapidEnemyTank
HRESULT RapidEnemyTank::Init(TILE_INFO* tile)
{
	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 256, 8, 4, true, RGB(255, 0, 255));
	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }

	pos.x = TILEMAPTOOL_SIZE_X / 2.0f;
	pos.y = TILEMAPTOOL_SIZE_Y / 2.0f;

	bodySize = 64;
	moveSpeed = 2.0f;

	tileInfo = tile;

	SetShape();

	moveDir = MoveDir::Down;

	bIsAlive = true;

	ammoCount = 30;
	ammoPack = new Ammo[ammoCount];
	// �̻��� �ʱ�ȭ
	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Init();
	}

	return S_OK;
}
#pragma endregion

#pragma region DefensiveEnemyTank
HRESULT DefensiveEnemyTank::Init(TILE_INFO* tile)
{
	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 256, 8, 4, true, RGB(255, 0, 255));
	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	spawnImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Spawn_Effect.bmp");

	if (img == nullptr) { cout << "PlayerTankImg nullptr" << endl; return E_FAIL; }
	if (spawnImg == nullptr) { cout << "SpawnImg nullptr" << endl;  return E_FAIL; }

	pos.x = TILEMAPTOOL_SIZE_X / 2.0f;
	pos.y = TILEMAPTOOL_SIZE_Y / 2.0f;

	bodySize = 64;
	moveSpeed = 2.0f;

	tileInfo = tile;

	SetShape();

	moveDir = MoveDir::Down;

	bIsAlive = true;

	ammoCount = 30;
	ammoPack = new Ammo[ammoCount];
	// �̻��� �ʱ�ȭ
	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Init();
	}

	return S_OK;
}
#pragma endregion

void Tank::Update()
{
	if (bIsAlive == false)	return;
	SetShape();

	elapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
	if (bCheckSpawnStatus)
	{
		// Ÿ�̸Ӱ� 2�ʰ� �Ǹ� ������ ���� ����, ����ð� �ʱ�ȭ
		spawnElapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
		if (bCheckSpawnStatus && elapsedCount >= spawnTime) { elapsedCount -= spawnTime; bCheckSpawnStatus = false; }

		spawnElapsedCount += TimerManager::GetSingleton()->GetDeltaTime();
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

	if (!bCheckSpawnStatus)
	{
		Move();
		Fire();
	}
}

void Tank::Render(HDC hdc)
{
	if (bIsAlive == false)	return;

	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Render(hdc);
	}

	//Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);
	if (bCheckSpawnStatus)
	{
		spawnImg->Render(hdc, pos.x - bodySize * 0.25f, pos.y - bodySize * 0.25f, spawnImgFrame, 0, 1.0f);
	}
	else
	{
		img->Render(hdc, pos.x, pos.y, moveDir + checkMoveCount, (int)type, 0.5f);
	}
}

void Tank::Release()
{
	delete[] ammoPack;
}

void Tank::Move()
{
	if (elapsedCount >= delay)
	{
		elapsedCount = 0;
		delay = RANDOM(0, 3);
		moveDir = (MoveDir)(RANDOM(0, 3) * 2);
	}

	switch (moveDir)
	{
	case MoveDir::Left:
		if (moveDir == MoveDir::Up || moveDir == MoveDir::Down)
		{
			if ((int)pos.y % 16 <= CORRECTION_POS_MIN)
			{
				pos.y -= (int)pos.y % 16;
			}
			else if ((int)pos.y % 16 >= CORRECTION_POS_MAX)
			{
				pos.y += 16 - (int)pos.y % 16;
			}
		}

		pos.x -= moveSpeed;
		SetShape();
		if (IsCollided() || shape.left < STAGE_SIZE_X)
		{
			pos.x += moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }

		break;
	case MoveDir::Right:
		if (moveDir == MoveDir::Up || moveDir == MoveDir::Down)
		{
			if ((int)pos.y % 16 <= CORRECTION_POS_MIN)
			{
				pos.y -= (int)pos.y % 16;
			}
			else if ((int)pos.y % 16 >= CORRECTION_POS_MAX)
			{
				pos.y += 16 - (int)pos.y % 16;
			}
		}
		moveDir = MoveDir::Right;

		pos.x += moveSpeed;
		SetShape();

		if (IsCollided() || shape.right > 416 + STAGE_SIZE_X)
		{
			pos.x -= moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }
		break;
	case MoveDir::Up:
		if (moveDir == MoveDir::Left || moveDir == MoveDir::Right)
		{
			if ((int)pos.x % 16 <= CORRECTION_POS_MIN)
			{
				pos.x -= (int)pos.x % 16;
			}
			else if ((int)pos.x % 16 >= CORRECTION_POS_MAX)
			{
				pos.x += 16 - (int)pos.x % 16;
			}
		}
		moveDir = MoveDir::Up;

		pos.y -= moveSpeed;
		SetShape();
		if (IsCollided() || shape.top < STAGE_SIZE_Y)
		{
			pos.y += moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }
		break;
	case MoveDir::Down:
		if (moveDir == MoveDir::Left || moveDir == MoveDir::Right)
		{
			if ((int)pos.x % 16 <= CORRECTION_POS_MIN)
			{
				pos.x -= (int)pos.x % 16;
			}
			else if ((int)pos.x % 16 >= CORRECTION_POS_MAX)
			{
				pos.x += 16 - (int)pos.x % 16;
			}
		}
		moveDir = MoveDir::Down;

		pos.y += moveSpeed;
		SetShape();
		if (IsCollided() || shape.bottom > 416 + STAGE_SIZE_Y)
		{
			pos.y -= moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }
		break;
	default:
		break;
	}
}

bool Tank::IsCollided()
{
	RECT temp = {};

	for (int i = 0; i < TILE_COUNT_Y * TILE_COUNT_X; i++)
	{
		if (IntersectRect(&temp, &tileInfo[i].collider, &shape))
		{
			return true;
		}
	}

	return false;
}

void Tank::SetShape()
{
	shape.left = pos.x - (bodySize / 2);
	shape.top = pos.y - (bodySize / 2);
	shape.right = shape.left + bodySize / 2;
	shape.bottom = shape.top + bodySize / 2;
}
