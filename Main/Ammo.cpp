#include "Ammo.h"
#include "Tank.h"
#include "Image.h"
#include "CommonFunction.h"

HRESULT Ammo::Init(TILE_INFO* tile/*, EnemyManager* enemyMgr, Tank* playerTank*/)
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

	moveSpeed = 200.0f;		// �ʴ� 15�ȼ� �̵�
	moveAngle = 0.0f;

	target = nullptr;

	isFire = false;
	//isAlive = true;

	ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Down.bmp", 6, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Left.bmp", 6, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Right.bmp", 6, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Up.bmp", 6, 8, true, RGB(255, 0, 255));
	img = ImageManager::GetSingleton()->FindImage("Image/Bullet/Missile_Up.bmp");
	 
	if (img == nullptr)
	{
		return E_FAIL;
	}

	collision.left = pos.x - (bodySize / 2.0f);
	collision.top = pos.y - (bodySize / 2.0f);
	collision.right = pos.x + (bodySize / 2.0f);
	collision.bottom = pos.y + (bodySize / 2.0f);

	bulletDir = BulletDir::Up;

	this->tile = tile;
	//this->tile = enemyMgr;
	//this->target = playerTank;

	return S_OK;
}

void Ammo::Update()
{
	//if (isAlive == false)	return;

	if (isFire)
	{	

		pos.x += cos(moveAngle) * moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();		// �����Ӵ� �̵��Ÿ� -> �ð� �� �̵��Ÿ�
		pos.y -= sin(moveAngle) * moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();


		collision.left = pos.x - (bodySize / 2.0f);
		collision.top = pos.y - (bodySize / 2.0f);
		collision.right = pos.x + (bodySize / 2.0f);
		collision.bottom = pos.y + (bodySize / 2.0f);

		int posIdX = (pos.x- STAGE_SIZE_X) / 16;
		int posIdY = (pos.y- STAGE_SIZE_Y) / 16;

		cout << "1X : " <<posIdX << endl;
		cout << "1Y : " << posIdY << endl;
		cout << "2X : " << posIdX-1 << endl;
		cout << "2Y : " << posIdY+1 << endl;
		// Ÿ�ٰ��� �浹Ȯ��
		if (CheckCollision(posIdX, posIdY))
		{
			isFire = false;
		}

		// ȭ���� ������� Ȯ��
		if (collision.left > STAGE_SIZE_X + 416 || collision.right < STAGE_SIZE_X ||
			collision.top > STAGE_SIZE_Y  + 416 || collision.bottom < STAGE_SIZE_Y)
		{
			collision.left = -10;
			collision.top = -10;
			collision.right = -10;
			collision.bottom = -10;
			pos.x = -10;
			pos.y = -10;
			isFire = false;
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

	if (bulletDir == BulletDir::Up || bulletDir == BulletDir::Down)
	{
		if (IntersectRect(&rc, &collision, &(tile[26 * idY + idX - 1].collider)))
		{
			// �� ���ֱ�
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
		}





		if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)))
		{
			// �� ���ֱ�
			cout << "�� 2 �ε�����" << endl;
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
		}



		if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX + 1)].collider)))
		{
			// �� ���ֱ�
			cout << "�� 3 �ε�����" << endl;
			check = true;
			if (bulletDir == BulletDir::Down && tile[26 * (idY)+idX + 1].tileType == TileType::Brick)
			{
				tile[26 * (idY)+idX + 1].collider.top += 8;
				tile[26 * (idY)+idX + 1].topHit++;
			}
			else if (bulletDir == BulletDir::Up && tile[26 * (idY)+idX + 1].tileType == TileType::Brick)
			{
				tile[26 * (idY)+idX + 1].collider.bottom -= 8;
				tile[26 * (idY)+idX + 1].bottomHit++;
			}
		}
	}
	else if(bulletDir == BulletDir::Left || bulletDir == BulletDir::Right)
	{
		if (IntersectRect(&rc, &collision, &(tile[26 * (idY-1) + idX].collider)) )
		{
			// �� ���ֱ�
			cout << "�� 1 �ε�����" << endl;
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
		}
		if (IntersectRect(&rc, &collision, &(tile[26 * (idY)+(idX)].collider)))
		{
			// �� ���ֱ�
			cout << "�� 2 �ε�����" << endl;
			check = true;
			if (bulletDir == BulletDir::Left && tile[26 * (idY) + idX].tileType == TileType::Brick )
			{
				tile[26 * (idY) + idX].collider.right -= 8;
				tile[26 * (idY) + idX].rightHit++;
			}
			else if (bulletDir == BulletDir::Right && tile[26 * (idY) + idX].tileType == TileType::Brick)
			{
				tile[26 * (idY) + idX].collider.left += 8;
				tile[26 * (idY) + idX].leftHit++;
			}
		}
		if (IntersectRect(&rc, &collision, &(tile[26 * (idY+1)+(idX)].collider)))
		{
			// �� ���ֱ�
			cout << "�� 3 �ε�����" << endl;
			check = true;
			if (bulletDir == BulletDir::Left && tile[26 * (idY + 1) + idX].tileType == TileType::Brick)
			{
				tile[26 * (idY + 1)+idX].collider.right -= 8;
				tile[26 * (idY + 1) + idX].rightHit++;
			}
			else if (bulletDir == BulletDir::Right && tile[26 * (idY + 1) + idX].tileType == TileType::Brick)
			{
				tile[26 * (idY + 1) + idX].collider.left += 8;
				tile[26 * (idY + 1) + idX].leftHit++;
			}
		}
		
	}
	
	
	if (check)
	{
		collision.left = -10;
		collision.top = -10;
		collision.right = -10;
		collision.bottom = -10;
		pos.x = -10;
		pos.y = -10;
		return true;
	}

	return false;
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

Ammo::Ammo()
{
}

Ammo::~Ammo()
{
}
