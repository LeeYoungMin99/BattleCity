#include "Tank.h"
#include "Image.h"


#pragma region PlyaerTank
HRESULT PlayerTank::Init(TILE_INFO* tile)
{
	ImageManager::GetSingleton()->AddImage("Image/Player/Player.bmp", 512, 256, 8, 4, true, RGB(255, 0, 255));
	img = ImageManager::GetSingleton()->FindImage("Image/Player/Player.bmp");
	if (img == nullptr)
	{
		return E_FAIL;
	}

	pos.x = 200;
	pos.y = 430;

	bodySize = 64;
	moveSpeed = 2.0f;

	tileInfo = tile;

	SetShape();

	moveDir = MoveDir::Up;

	bIsAlive = true;

	ammoCount = 1;
	ammoPack = new Ammo[ammoCount];
	// 미사일 초기화
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



	SetShape();
	Move();
	Fire();
}

void PlayerTank::Render(HDC hdc)
{
	if (bIsAlive == false)	return;

	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Render(hdc);
	}

	Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);
	img->Render(hdc, pos.x, pos.y, moveDir + checkMoveCount, enforceCount, 0.5f);
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
			if ((int)pos.y % 16 <= 4)
			{
				pos.y -= (int)pos.y % 16;
			}
			else if ((int)pos.y % 16 >= 12)
			{
				pos.y += 16 - (int)pos.y % 16;
			}
		}
		moveDir = MoveDir::Left;

		pos.x -= moveSpeed;
		SetShape();
		if (IsCollided() || shape.left < 0)
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
			if ((int)pos.y % 16 <= 4)
			{
				pos.y -= (int)pos.y % 16;
			}
			else if ((int)pos.y % 16 >= 12)
			{
				pos.y += 16 - (int)pos.y % 16;
			}
		}
		moveDir = MoveDir::Right;

		pos.x += moveSpeed;
		SetShape();

		if (IsCollided() || shape.right > 416)
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
			if ((int)pos.x % 16 <= 4)
			{
				pos.x -= (int)pos.x % 16;
			}
			else if ((int)pos.x % 16 >= 12)
			{
				pos.x += 16 - (int)pos.x % 16;
			}
		}
		moveDir = MoveDir::Up;

		pos.y -= moveSpeed;
		SetShape();
		if (IsCollided() || shape.top < 0)
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
			if ((int)pos.x % 16 <= 4)
			{
				pos.x -= (int)pos.x % 16;
			}
			else if ((int)pos.x % 16 >= 12)
			{
				pos.x += 16 - (int)pos.x % 16;
			}
		}
		moveDir = MoveDir::Down;

		pos.y += moveSpeed;
		SetShape();
		if (IsCollided() || shape.bottom > 416)
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
			// 전체 미사일을 순회하면서 발사 됐는지 안됐는지 판단
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
			ammoPack[i].SetPos(BarrelPos);	// 미사일 위치 변경
			ammoPack[i].SetIsFire(true);	// 미사일 상태 변경

			break;
		}
	}
}
#pragma endregion

#pragma region NormalEnemyTank
HRESULT NormalEnemyTank::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));

	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	if (img == nullptr)
	{
		return E_FAIL;
	}

	pos.x = TILEMAPTOOL_SIZE_X / 2.0f;
	pos.y = TILEMAPTOOL_SIZE_Y / 2.0f;

	bodySize = 64;
	moveSpeed = 2.0f;

	SetShape();

	moveDir = MoveDir::Down;

	bIsAlive = true;

	ammoCount = 30;
	ammoPack = new Ammo[ammoCount];
	// 미사일 초기화
	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Init();
	}

	return S_OK;
}
#pragma endregion

#pragma region SpeedEnemyTank
HRESULT SpeedEnemyTank::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));

	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	if (img == nullptr)
	{
		return E_FAIL;
	}

	pos.x = TILEMAPTOOL_SIZE_X / 2.0f;
	pos.y = TILEMAPTOOL_SIZE_Y / 2.0f;

	bodySize = 64;
	moveSpeed = 2.0f;

	SetShape();

	moveDir = MoveDir::Down;

	bIsAlive = true;

	ammoCount = 30;
	ammoPack = new Ammo[ammoCount];
	// 미사일 초기화
	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Init();
	}

	return S_OK;
}
#pragma endregion

#pragma region RapidEnemyTank
HRESULT RapidEnemyTank::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));

	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	if (img == nullptr)
	{
		return E_FAIL;
	}

	pos.x = TILEMAPTOOL_SIZE_X / 2.0f;
	pos.y = TILEMAPTOOL_SIZE_Y / 2.0f;

	bodySize = 64;
	moveSpeed = 2.0f;

	SetShape();

	moveDir = MoveDir::Down;

	bIsAlive = true;

	ammoCount = 30;
	ammoPack = new Ammo[ammoCount];
	// 미사일 초기화
	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Init();
	}

	return S_OK;
}
#pragma endregion

#pragma region DefensiveEnemyTank
HRESULT DefensiveEnemyTank::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));

	img = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	if (img == nullptr)
	{
		return E_FAIL;
	}

	pos.x = TILEMAPTOOL_SIZE_X / 2.0f;
	pos.y = TILEMAPTOOL_SIZE_Y / 2.0f;

	bodySize = 64;
	moveSpeed = 2.0f;

	SetShape();

	moveDir = MoveDir::Down;

	bIsAlive = true;

	ammoCount = 30;
	ammoPack = new Ammo[ammoCount];
	// 미사일 초기화
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
	Move();
	Fire();
}

void Tank::Render(HDC hdc)
{
	if (bIsAlive == false)	return;

	for (int i = 0; i < ammoCount; i++)
	{
		ammoPack[i].Render(hdc);
	}

	img->Render(hdc, pos.x, pos.y, moveDir + checkMoveCount, (int)type, 0.5f);
}

void Tank::Release()
{
	delete[] ammoPack;
}

void Tank::Move()
{
	// 한방향으로 이동중일떄
	// 일정 프레임동안 이동이 안되면
	// 방향을 전환.
	// 리스폰떄 아래방향 보고있음.

	if (elapsedCount == delay)
	{
		moveDir = (MoveDir)((rand() % 4) * 2);
	}

	switch (moveDir)
	{
	case MoveDir::Left:
		if (moveDir == MoveDir::Up || moveDir == MoveDir::Down)
		{
			if ((int)pos.y % 16 <= 4)
			{
				pos.y -= (int)pos.y % 16;
			}
			else if ((int)pos.y % 16 >= 12)
			{
				pos.y += 16 - (int)pos.y % 16;
			}
		}

		pos.x -= moveSpeed;
		SetShape();
		if (IsCollided() || shape.left < 0)
		{
			pos.x += moveSpeed;
			SetShape();
		}

		if (checkMoveCount > 0) { checkMoveCount = 0; }
		else { checkMoveCount = 1; }

		break;
	case MoveDir::Right:

		break;
	case MoveDir::Up:

		break;
	case MoveDir::Down:

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
