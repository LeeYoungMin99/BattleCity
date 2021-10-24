#pragma once
#include "Ammo.h"
#include "Tank.h"
#include "Image.h"
#include "EnemyManager.h"
#include "AmmoManager.h"
#include "CommonFunction.h"

HRESULT Ammo::Init(TILE_INFO* tile, Tank* ownerTank, vector<Tank*>* enemyTanks, Tank* playerTank)
{
	pos.x = 0.0f;
	pos.y = 0.0f;

	bodySize = 10;
	shape.left = 0;
	shape.top = 0;
	shape.right = 0;
	shape.bottom = 0;

	moveSpeed = 200.0f;
	moveAngle = 0.0f;

	isFire = false;


	img = ImageManager::GetSingleton()->FindImage("Image/Bullet/Missile_Up.bmp");

	boomImg = ImageManager::GetSingleton()->FindImage("Image/Effect/Boom_Effect.bmp");

	if (img == nullptr)
	{
		return E_FAIL;
	}

	collision.left = (LONG)(pos.x - (bodySize / 2.0f));
	collision.top = (LONG)(pos.y - (bodySize / 2.0f));
	collision.right = (LONG)(pos.x + (bodySize / 2.0f));
	collision.bottom = (LONG)(pos.y + (bodySize / 2.0f));

	bulletDir = BulletDir::Up;

	bRenderBoomImg = false;

	this->tile = tile;
	this->ownerTank = ownerTank;
	this->playerTank = playerTank;
	this->enemyTanks = enemyTanks;
	return S_OK;
}

void Ammo::Update()
{
	if (isFire)
	{
		if (!bRenderBoomImg)
		{
			pos.x += cos(moveAngle) * moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();
			pos.y -= sin(moveAngle) * moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();
			SetCollider();
		}

		int posIdX = (int)((pos.x - STAGE_SIZE_X) / 16);
		int posIdY = (int)((pos.y - STAGE_SIZE_Y) / 16);

		if (collision.left > STAGE_SIZE_X + 416 || collision.right < STAGE_SIZE_X ||
			collision.top > STAGE_SIZE_Y + 416 || collision.bottom < STAGE_SIZE_Y)
		{
			bRenderBoomImg = true;
		}

		if (CheckCollision(posIdX, posIdY))
		{
			bRenderBoomImg = true;
			collision.left = 0;
			collision.top = 0;
			collision.right = 0;
			collision.bottom = 0;
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

					ownerTank->SubtractCurrFireNumberOfAmmo(1);
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
	if (isFire)
	{
		Rectangle(hdc, collision.left, collision.top, collision.right, collision.bottom);
		img->Render(hdc, (int)pos.x, (int)pos.y);
		if (bRenderBoomImg)
		{
			boomImg->Render(hdc, (int)pos.x, (int)pos.y, boomImgCurrFrame, 0);
		}
	}
}

void Ammo::Release()
{
	tile = nullptr;
	ownerTank = nullptr;
	playerTank = nullptr;
	img = nullptr;
	boomImg = nullptr;
}

bool Ammo::CheckCollision(int idX, int idY)
{
	RECT rc;
	int check = false;

	if (ownerTank->GetEnforceCount() != 3)
	{
		if (bulletDir == BulletDir::Up || bulletDir == BulletDir::Down)
		{
			if (IntersectRect(&rc, &collision, &(tile[26 * idY + idX - 1].collider)) && tile[26 * idY + idX - 1].tileType != TileType::Water)
			{
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
				check = true;

				if (bulletDir == BulletDir::Down && (tile[26 * (idY)+idX - 1].tileType == TileType::Brick || tile[26 * (idY)+idX - 1].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX - 1].collider.top += 16;
					tile[26 * (idY)+idX - 1].topHit += 2;
				}
				else if (bulletDir == BulletDir::Up && (tile[26 * (idY)+idX - 1].tileType == TileType::Brick || tile[26 * (idY)+idX - 1].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX - 1].collider.bottom -= 16;
					tile[26 * (idY)+idX - 1].bottomHit += 2;
				}


				if (tile[26 * (idY)+idX - 1].topHit + tile[26 * (idY)+idX - 1].bottomHit >= 2)
				{
					tile[26 * (idY)+idX - 1].bodyCollider.left = 0;
					tile[26 * (idY)+idX - 1].bodyCollider.right = 0;
				}
			}
			if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)))
			{
				check = true;
				if (bulletDir == BulletDir::Down && (tile[26 * (idY)+idX].tileType == TileType::Brick || tile[26 * (idY)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX].collider.top += 16;
					tile[26 * (idY)+idX].topHit += 2;
				}
				else if (bulletDir == BulletDir::Up && (tile[26 * (idY)+idX].tileType == TileType::Brick || tile[26 * (idY)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX].collider.bottom -= 16;
					tile[26 * (idY)+idX].bottomHit += 2;
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
				check = true;
				if (bulletDir == BulletDir::Left && (tile[26 * (idY - 1) + idX].tileType == TileType::Brick || tile[26 * (idY - 1) + idX].tileType == TileType::Wall))
				{
					tile[26 * (idY - 1) + idX].collider.right -= 16;
					tile[26 * (idY - 1) + idX].rightHit += 2;
				}
				else if (bulletDir == BulletDir::Right && (tile[26 * (idY - 1) + idX].tileType == TileType::Brick || tile[26 * (idY - 1) + idX].tileType == TileType::Wall))
				{
					tile[26 * (idY - 1) + idX].collider.left += 16;
					tile[26 * (idY - 1) + idX].leftHit += 2;
				}
				if (tile[26 * (idY - 1) + idX].leftHit + tile[26 * (idY - 1) + idX].rightHit >= 2)
				{
					tile[26 * (idY - 1) + idX].bodyCollider.left = 0;
					tile[26 * (idY - 1) + idX].bodyCollider.right = 0;
				}
			}
			if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)))
			{
				check = true;
				if (bulletDir == BulletDir::Left && (tile[26 * (idY)+idX].tileType == TileType::Brick || tile[26 * (idY)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX].collider.right -= 16;
					tile[26 * (idY)+idX].rightHit += 2;
				}
				else if (bulletDir == BulletDir::Right && (tile[26 * (idY)+idX].tileType == TileType::Brick || tile[26 * (idY)+idX].tileType == TileType::Wall))
				{
					tile[26 * (idY)+idX].collider.left += 16;
					tile[26 * (idY)+idX].leftHit += 2;
				}
				if (tile[26 * (idY)+idX].leftHit + tile[26 * (idY)+idX].rightHit >= 2)
				{
					tile[26 * (idY)+idX].bodyCollider.left = 0;
					tile[26 * (idY)+idX].bodyCollider.right = 0;
				}
			}

		}
	}
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
		}
	}


	if (playerTank == nullptr)
	{
		for (itEnemyTanks = enemyTanks->begin();
			itEnemyTanks != enemyTanks->end(); itEnemyTanks++)
		{
			if (!(*itEnemyTanks)->GetCheckSpawnStatus() && IntersectRect(&rc, (*itEnemyTanks)->GetShapeAddress(), &collision))
			{
				if (!CheckHitTank((*itEnemyTanks)))
				{
					hitTankList.push_back((*itEnemyTanks));
					(*itEnemyTanks)->SubtractHP(1);
					if ((*itEnemyTanks)->GetHP() == 0)
					{
						(*itEnemyTanks)->increaseScore();
						GameManager::GetSingleton()->remainMonster--;
					}
				}
				check = true;
			}
		}
	}

	if (playerTank != nullptr && !(playerTank->GetCheckSpawnStatus()))
	{
		if (IntersectRect(&rc, playerTank->GetShapeAddress(), &collision))
		{
			if (((PlayerTank*)playerTank)->GetCheckShieldOn())
			{
				isFire = false;
				collision.left = -10;
				collision.top = -10;
				collision.right = -10;
				collision.bottom = -10;
				pos.x = -10;
				pos.y = -10;
				bRenderBoomImg = false;
				ownerTank->SubtractCurrFireNumberOfAmmo(1);
			}
			else
			{
				playerTank->SubtractHP(1);

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
				bRenderBoomImg = false;
				ownerTank->SubtractCurrFireNumberOfAmmo(1);
				(*itAmmos)->isFire = false;
				(*itAmmos)->collision.left = -50;
				(*itAmmos)->collision.top = -50;
				(*itAmmos)->collision.right = -50;
				(*itAmmos)->collision.bottom = -50;
				(*itAmmos)->pos.x = -50;
				(*itAmmos)->pos.y = -50;
				(*itAmmos)->ownerTank->SubtractCurrFireNumberOfAmmo(1);
				(*itAmmos)->bRenderBoomImg = false;
			}
		}
	}

	if (check)
	{
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
		SetMoveAngle((float)DEGREE_TO_RADIAN(180));
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
		SetMoveAngle((float)DEGREE_TO_RADIAN(90));
	}
	else if (dir._Equal("Down"))
	{
		bulletDir = BulletDir::Down;
		img = ImageManager::GetSingleton()->FindImage("Image/Bullet/Missile_Down.bmp");
		SetMoveAngle((float)DEGREE_TO_RADIAN(270));
	}
}

void Ammo::SetCollider()
{
	if (bulletDir == BulletDir::Left)
	{
		collision.left = (LONG)(pos.x - (bodySize / 2.0f) + 3);
		collision.top = (LONG)(pos.y - (bodySize / 2.0f));
		collision.right = (LONG)(pos.x + (bodySize / 2.0f) + 1);
		collision.bottom = (LONG)(pos.y + (bodySize / 2.0f));
	}
	else if (bulletDir == BulletDir::Right)
	{
		collision.left = (LONG)(pos.x - (bodySize / 2.0f) - 1);
		collision.top = (LONG)(pos.y - (bodySize / 2.0f));
		collision.right = (LONG)(pos.x + (bodySize / 2.0f) - 3);
		collision.bottom = (LONG)(pos.y + (bodySize / 2.0f));
	}
	else if (bulletDir == BulletDir::Down)
	{
		collision.left = (LONG)(pos.x - (bodySize / 2.0f));
		collision.top = (LONG)(pos.y - (bodySize / 2.0f) - 1);
		collision.right = (LONG)(pos.x + (bodySize / 2.0f));
		collision.bottom = (LONG)(pos.y + (bodySize / 2.0f) - 3);
	}
	else if (bulletDir == BulletDir::Up)
	{
		collision.left = (LONG)(pos.x - (bodySize / 2.0f));
		collision.top = (LONG)(pos.y - (bodySize / 2.0f) + 4);
		collision.right = (LONG)(pos.x + (bodySize / 2.0f));
		collision.bottom = (LONG)(pos.y + (bodySize / 2.0f) + 1);
	}
}

Ammo::Ammo()
{
}