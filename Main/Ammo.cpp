#include "Ammo.h"
#include "Tank.h"
#include "Image.h"
#include "EnemyManager.h"
#include "AmmoManager.h"
#include "CommonFunction.h"

HRESULT Ammo::Init(TILE_INFO* tile, Tank* ownerTank, EnemyManager* enemyMgr, Tank* playerTank)
{
	//float x = 10.0f, y = 20.0f, h = 0.0f;
	//h = (float)sqrtf((x * x) + (y * y));

	pos.x = 0.0f;
	pos.y = 0.0f;

	bodySize = 10;
	shape.left = 0;
	shape.top = 0;
	shape.right = 0;
	shape.bottom = 0;

	moveSpeed = 200.0f;		// 초당 15픽셀 이동
	moveAngle = 0.0f;

	isFire = false;
	//isAlive = true;

	ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Down.bmp", 6, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Left.bmp", 6, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Right.bmp", 6, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Up.bmp", 6, 8, true, RGB(255, 0, 255));
	img = ImageManager::GetSingleton()->FindImage("Image/Bullet/Missile_Up.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Effect/Boom_Effect.bmp", 96, 32, 3, 1, true, RGB(255, 0, 255));
	boomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Boom_Effect.bmp");

	if (img == nullptr)
	{
		return E_FAIL;
	}

	collision.left = pos.x - (bodySize / 2.0f);
	collision.top = pos.y - (bodySize / 2.0f);
	collision.right = pos.x + (bodySize / 2.0f);
	collision.bottom = pos.y + (bodySize / 2.0f);

	bulletDir = BulletDir::Up;

	bRenderBoomImg = false;

	this->tile = tile;
	this->ownerTank = ownerTank;
	this->enemyMgr = enemyMgr;
	this->playerTank = playerTank;

	vecEnemyTanks = this->enemyMgr->GetAddresVecEnemys();
	return S_OK;
}

void Ammo::Update()
{
	//if (isAlive == false)	return;

	if (isFire)
	{
		if (!bRenderBoomImg)
		{
			pos.x += cos(moveAngle) * moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();		// 프레임당 이동거리 -> 시간 당 이동거리
			pos.y -= sin(moveAngle) * moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();
		}

		SetCollider();

		int posIdX = (pos.x - STAGE_SIZE_X) / 16;
		int posIdY = (pos.y - STAGE_SIZE_Y) / 16;

		// 화면을 벗어났는지 확인
		if (collision.left > STAGE_SIZE_X + 416 || collision.right < STAGE_SIZE_X ||
			collision.top > STAGE_SIZE_Y + 416 || collision.bottom < STAGE_SIZE_Y)
		{
			bRenderBoomImg = true;
		}

		// 타겟과의 충돌확인
		if (CheckCollision(posIdX, posIdY))
		{
			bRenderBoomImg = true;
		}

		if (bRenderBoomImg)
		{
			currElapsedCount++;

			if (currElapsedCount >= addImgFrameCount)
			{
				boomImgCurrFrame++;
				currElapsedCount = 0;

				if (boomImgCurrFrame == boomImgMaxFrame)
				{
					collision.left = -10;
					collision.top = -10;
					collision.right = -10;
					collision.bottom = -10;
					pos.x = -10;
					pos.y = -10;

					ownerTank->currFireNumberOfAmmo--;
					boomImgCurrFrame = 0;
					bRenderBoomImg = false;
					isFire = false;
					hitTankList.clear();
				}
			}
		}
	}
}

void Ammo::SetIsFire(bool fire)
{
	this->isFire = fire;
}

void Ammo::Render(HDC hdc)
{
	//if (isAlive == false)	return;

	if (isFire)
	{
		Rectangle(hdc, collision.left, collision.top, collision.right, collision.bottom);
		img->Render(hdc, pos.x, pos.y);
		//Ellipse(hdc, shape.left, shape.top, shape.right, shape.bottom);
		if (bRenderBoomImg)
		{
			boomImg->Render(hdc, pos.x, pos.y, boomImgCurrFrame, 0);
		}
	}
}

void Ammo::Release()
{
}

bool Ammo::CheckCollision(int idX, int idY)
{
	RECT rc;
	int check = false;

	/*if (IntersectRect(&rc, &collision, &target->GetShape()))
		return true;*/
	if (ownerTank->enforceCount != 3) 
	{
		if (bulletDir == BulletDir::Up || bulletDir == BulletDir::Down)
		{

			if (IntersectRect(&rc, &collision, &(tile[26 * idY + idX - 1].collider)) && tile[26 * idY + idX - 1].tileType != TileType::Water)
			{
				// 벽 없애기
				check = true;

				if (bulletDir == BulletDir::Down && tile[26 * (idY)+idX - 1].tileType == TileType::Brick)
				{
					tile[26 * (idY)+idX - 1].collider.top += 8;
					tile[26 * (idY)+idX - 1].topHit++;
				}
				else if (bulletDir == BulletDir::Up && tile[26 * (idY)+idX - 1].tileType == TileType::Brick)
				{
					tile[26 * (idY)+idX - 1].collider.bottom -= 8;
					tile[26 * (idY)+idX - 1].bottomHit++;
				}


				if (tile[26 * (idY)+idX - 1].topHit + tile[26 * (idY)+idX - 1].bottomHit >= 2)
				{
					tile[26 * (idY)+idX - 1].bodyCollider.left = 0;
					tile[26 * (idY)+idX - 1].bodyCollider.right = 0;
				}
			}

			if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)))
			{
				// 벽 없애기
				check = true;
				if (bulletDir == BulletDir::Down && tile[26 * (idY)+idX].tileType == TileType::Brick)
				{
					tile[26 * (idY)+idX].collider.top += 8;
					tile[26 * (idY)+idX].topHit++;
				}
				else if (bulletDir == BulletDir::Up && tile[26 * (idY)+idX].tileType == TileType::Brick)
				{
					tile[26 * (idY)+idX].collider.bottom -= 8;
					tile[26 * (idY)+idX].bottomHit++;
				}
				if (tile[26 * (idY)+idX].topHit + tile[26 * (idY)+idX].bottomHit >= 2)
				{
					tile[26 * (idY)+idX].bodyCollider.left = 0;
					tile[26 * (idY)+idX].bodyCollider.right = 0;
				}
			}


		}
		else if (bulletDir == BulletDir::Left || bulletDir == BulletDir::Right)
		{
			if (IntersectRect(&rc, &collision, &(tile[26 * (idY - 1) + idX].collider)))
			{
				// 벽 없애기
				check = true;
				if (bulletDir == BulletDir::Left && tile[26 * (idY - 1) + idX].tileType == TileType::Brick)
				{
					tile[26 * (idY - 1) + idX].collider.right -= 8;
					tile[26 * (idY - 1) + idX].rightHit++;
				}
				else if (bulletDir == BulletDir::Right && tile[26 * (idY - 1) + idX].tileType == TileType::Brick)
				{
					tile[26 * (idY - 1) + idX].collider.left += 8;
					tile[26 * (idY - 1) + idX].leftHit++;
				}
				if (tile[26 * (idY - 1) + idX].leftHit + tile[26 * (idY - 1) + idX].rightHit >= 2)
				{
					tile[26 * (idY - 1) + idX].bodyCollider.left = 0;
					tile[26 * (idY - 1) + idX].bodyCollider.right = 0;
				}
			}
			if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)))
			{
				// 벽 없애기
				check = true;
				if (bulletDir == BulletDir::Left && tile[26 * (idY)+idX].tileType == TileType::Brick)
				{
					tile[26 * (idY)+idX].collider.right -= 8;
					tile[26 * (idY)+idX].rightHit++;
				}
				else if (bulletDir == BulletDir::Right && tile[26 * (idY)+idX].tileType == TileType::Brick)
				{
					tile[26 * (idY)+idX].collider.left += 8;
					tile[26 * (idY)+idX].leftHit++;
				}
				if (tile[26 * (idY)+idX].leftHit + tile[26 * (idY)+idX].rightHit >= 2)
				{
					tile[26 * (idY)+idX].bodyCollider.left = 0;
					tile[26 * (idY)+idX].bodyCollider.right = 0;
				}
			}

		}
	}
	else
	{

		if (bulletDir == BulletDir::Up || bulletDir == BulletDir::Down)
		{

			if (IntersectRect(&rc, &collision, &(tile[26 * idY + idX - 1].collider)) && tile[26 * idY + idX - 1].tileType != TileType::Water)
			{
				// 벽 없애기
				check = true;

				if (bulletDir == BulletDir::Down && (tile[26 * (idY)+idX - 1].tileType == TileType::Brick || tile[26 * (idY)+idX - 1].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX - 1].collider.top += 16;
					tile[26 * (idY)+idX - 1].topHit+=2;
				}
				else if (bulletDir == BulletDir::Up && (tile[26 * (idY)+idX - 1].tileType == TileType::Brick || tile[26 * (idY)+idX - 1].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX - 1].collider.bottom -= 16;
					tile[26 * (idY)+idX - 1].bottomHit+=2;
				}


				if (tile[26 * (idY)+idX - 1].topHit + tile[26 * (idY)+idX - 1].bottomHit >= 2)
				{
					tile[26 * (idY)+idX - 1].bodyCollider.left = 0;
					tile[26 * (idY)+idX - 1].bodyCollider.right = 0;
				}
			}
			if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)))
			{
				// 벽 없애기
				check = true;
				if (bulletDir == BulletDir::Down && (tile[26 * (idY)+idX].tileType == TileType::Brick || tile[26 * (idY)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX].collider.top += 16;
					tile[26 * (idY)+idX].topHit+=2;
				}
				else if (bulletDir == BulletDir::Up && (tile[26 * (idY)+idX].tileType == TileType::Brick || tile[26 * (idY)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX].collider.bottom -= 16;
					tile[26 * (idY)+idX].bottomHit+=2;
				}
				if (tile[26 * (idY)+idX].topHit + tile[26 * (idY)+idX].bottomHit >= 2)
				{
					tile[26 * (idY)+idX].bodyCollider.left = 0;
					tile[26 * (idY)+idX].bodyCollider.right = 0;
				}
			}


		}
		else if (bulletDir == BulletDir::Left || bulletDir == BulletDir::Right)
		{
			if (IntersectRect(&rc, &collision, &(tile[26 * (idY - 1) + idX].collider)))
			{
				// 벽 없애기
				check = true;
				// 벽 없애기
				if (bulletDir == BulletDir::Left && (tile[26 * (idY-1)+idX].tileType == TileType::Brick || tile[26 * (idY-1)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY - 1) + idX].collider.right -= 16;
					tile[26 * (idY - 1) + idX].rightHit+=2;
				}
				else if (bulletDir == BulletDir::Right && (tile[26 * (idY-1)+idX].tileType == TileType::Brick || tile[26 * (idY-1)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY - 1) + idX].collider.left += 16;
					tile[26 * (idY - 1) + idX].leftHit+=2;
				}
				if (tile[26 * (idY - 1) + idX].leftHit + tile[26 * (idY - 1) + idX].rightHit >= 2)
				{
					tile[26 * (idY - 1) + idX].bodyCollider.left = 0;
					tile[26 * (idY - 1) + idX].bodyCollider.right = 0;
				}
			}
			if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)))
			{
				// 벽 없애기
				check = true;
				if (bulletDir == BulletDir::Left && (tile[26 * (idY)+idX].tileType == TileType::Brick || tile[26 * (idY)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX].collider.right -= 16;
					tile[26 * (idY)+idX].rightHit+=2;
				}
				else if (bulletDir == BulletDir::Right && (tile[26 * (idY)+idX].tileType == TileType::Brick || tile[26 * (idY)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX].collider.left += 16;
					tile[26 * (idY)+idX].leftHit+=2;
				}
				if (tile[26 * (idY)+idX].leftHit + tile[26 * (idY)+idX].rightHit >= 2)
				{
					tile[26 * (idY)+idX].bodyCollider.left = 0;
					tile[26 * (idY)+idX].bodyCollider.right = 0;
				}
			}

		}
	}
	// 넥서스 충돌 처리
	if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)) && GameManager::GetSingleton()->state != GameState::GameOver)
	{

		check = true;
		if (tile[26 * (idY)+(idX)].tileType == TileType::Nexus)
		{
			int index = 4;
			for (int i = 0; i < TILE_COUNT_X * TILE_COUNT_Y; i++)
			{
				if (tile[i].tileType == TileType::Nexus)
				{
					tile[i].frameX = index++;
					tile[i].frameY = 1;

				}
			}
			GameManager::GetSingleton()->state = GameState::DestoryNexus;
			//조건 처리
		}
	}


	if (playerTank == nullptr)
	{	// 플레이어 탱크면
		for (itEnemyTanks = vecEnemyTanks->begin();
			itEnemyTanks != vecEnemyTanks->end(); itEnemyTanks++)
		{	// Enemy가 Spawn상태가 아니라면 충돌 처리
			if (!(*itEnemyTanks)->bCheckSpawnStatus && IntersectRect(&rc, (*itEnemyTanks)->GetShapeAddress(), &collision))
			{
				if (!CheckHitTank((*itEnemyTanks)))
				{
					hitTankList.push_back((*itEnemyTanks));
					(*itEnemyTanks)->HP--;
				}
				if ((*itEnemyTanks)->HP == 0)

				{
					(*itEnemyTanks)->increaseScore();
					GameManager::GetSingleton()->remainMonster--;
				}
				check = true;
			}
		}
	}

	if (playerTank != nullptr && !(playerTank->bCheckSpawnStatus))
	{	// Player를 타겟으로 잡고있고 Player가 Spawn상태가 아니라면 충돌처리 
		if (IntersectRect(&rc, playerTank->GetShapeAddress(), &collision))
		{
			if (playerTank->bCheckShieldOn)
			{
				isFire = false;
				collision.left = -10;
				collision.top = -10;
				collision.right = -10;
				collision.bottom = -10;
				pos.x = -10;
				pos.y = -10;
				ownerTank->currFireNumberOfAmmo--;
			}
			else
			{
				playerTank->HP--;

				check = true;
			}
		}
	}

	if (playerTank == nullptr)
	{
		for (itAmmos = targetAmmos->begin();
			itAmmos != targetAmmos->end(); itAmmos++)
		{
			if (IntersectRect(&rc, &((*itAmmos)->collision), &collision))
			{
				isFire = false;
				collision.left = -10;
				collision.top = -10;
				collision.right = -10;
				collision.bottom = -10;
				pos.x = -10;
				pos.y = -10;
				ownerTank->currFireNumberOfAmmo--;
				(*itAmmos)->isFire = false;
				(*itAmmos)->collision.left = -50;
				(*itAmmos)->collision.top = -50;
				(*itAmmos)->collision.right = -50;
				(*itAmmos)->collision.bottom = -50;
				(*itAmmos)->pos.x = -50;
				(*itAmmos)->pos.y = -50;
				(*itAmmos)->ownerTank->currFireNumberOfAmmo--;
			}
		}
	}

	//if (playerTank != nullptr)
	//{
	//	for (itAmmos = playerAmmoManager->vecAmmos.begin(); 
	//		itAmmos != playerAmmoManager->vecAmmos.end(); itAmmos++)
	//	{
	//		if (IntersectRect(&rc, &((*itAmmos)->collision), &collision))
	//		{
	//			isFire = false;
	//			collision.left = -50;
	//			collision.top = -50;
	//			collision.right = -50;
	//			collision.bottom = -50;
	//			pos.x = -50;
	//			pos.y = -50;
	//			ownerTank->currFireNomberOFAmmo--;
	//			(*itAmmos)->isFire = false;
	//			(*itAmmos)->collision.left = -10;
	//			(*itAmmos)->collision.top = -10;
	//			(*itAmmos)->collision.right = -10;
	//			(*itAmmos)->collision.bottom = -10;
	//			(*itAmmos)->pos.x = -10;
	//			(*itAmmos)->pos.y = -10;
	//			playerTank->currFireNomberOFAmmo--;
	//		}
	//	}
	//}

	if (check)
	{
		//collision.left = -10;
		//collision.top = -10;
		//collision.right = -10;
		//collision.bottom = -10;
		//pos.x = -10;
		//pos.y = -10;
		return true;
	}

	return false;
}

bool Ammo::CheckHitTank(Tank* enemyTank)
{
	{
		for (const auto& enemy : hitTankList)
		{
			if (enemy == enemyTank)
			{
				return true;
			}
		}
		return false;
	}
}

void Ammo::SetMoveDir(string dir)
{
	if (dir._Equal("Left"))
	{
		bulletDir = BulletDir::Left;
		img = ImageManager::GetSingleton()->FindImage("Image/Bullet/Missile_Left.bmp");
		SetMoveAngle(DEGREE_TO_RADIAN(180));
	}
	else if (dir._Equal("Right"))
	{
		bulletDir = BulletDir::Right;
		img = ImageManager::GetSingleton()->FindImage("Image/Bullet/Missile_Right.bmp");
		SetMoveAngle(0.0f);
	}
	else if (dir._Equal("Up"))
	{
		bulletDir = BulletDir::Up;
		img = ImageManager::GetSingleton()->FindImage("Image/Bullet/Missile_Up.bmp");
		SetMoveAngle(DEGREE_TO_RADIAN(90));
	}
	else if (dir._Equal("Down"))
	{
		bulletDir = BulletDir::Down;
		img = ImageManager::GetSingleton()->FindImage("Image/Bullet/Missile_Down.bmp");
		SetMoveAngle(DEGREE_TO_RADIAN(270));
	}
}

void Ammo::SetCollider()
{
	if (bulletDir == BulletDir::Left)
	{
		collision.left = pos.x - (bodySize / 2.0f) + 3;
		collision.top = pos.y - (bodySize / 2.0f);
		collision.right = pos.x + (bodySize / 2.0f) + 1;
		collision.bottom = pos.y + (bodySize / 2.0f);
	}
	else if (bulletDir == BulletDir::Right)
	{
		collision.left = pos.x - (bodySize / 2.0f) - 1;
		collision.top = pos.y - (bodySize / 2.0f);
		collision.right = pos.x + (bodySize / 2.0f) - 3;
		collision.bottom = pos.y + (bodySize / 2.0f);
	}
	else if (bulletDir == BulletDir::Down)
	{
		collision.left = pos.x - (bodySize / 2.0f);
		collision.top = pos.y - (bodySize / 2.0f) - 1;
		collision.right = pos.x + (bodySize / 2.0f);
		collision.bottom = pos.y + (bodySize / 2.0f) - 3;
	}
	else if (bulletDir == BulletDir::Up)
	{
		collision.left = pos.x - (bodySize / 2.0f);
		collision.top = pos.y - (bodySize / 2.0f) + 4;
		collision.right = pos.x + (bodySize / 2.0f);
		collision.bottom = pos.y + (bodySize / 2.0f) + 1;
	}
}

Ammo::Ammo()
{
}

Ammo::~Ammo()
{
}
