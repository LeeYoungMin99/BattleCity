#include "Ammo.h"
#include "Tank.h"
#include "Image.h"
#include "CommonFunction.h"

HRESULT Ammo::Init()
{
	//float x = 10.0f, y = 20.0f, h = 0.0f;
	//h = (float)sqrtf((x * x) + (y * y));


	pos.x = 0.0f;
	pos.y = 0.0f;

	bodySize = 21;

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
	img = ImageManager::GetSingleton()->AddImage("Image/Bullet/Missile_Up.bmp", 6, 8, true, RGB(255, 0, 255));
	 
	if (img == nullptr)
	{
		return E_FAIL;
	}



	bulletDir = BulletDir::Up;

	return S_OK;
}

void Ammo::Update()
{
	//if (isAlive == false)	return;

	if (isFire)
	{	
		cout << moveAngle << endl;

		pos.x += cos(moveAngle) * moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();		// �����Ӵ� �̵��Ÿ� -> �ð� �� �̵��Ÿ�
		pos.y -= sin(moveAngle) * moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();


		shape.left = pos.x - (bodySize / 2.0f);
		shape.top = pos.y - (bodySize / 2.0f);
		shape.right = pos.x + (bodySize / 2.0f);
		shape.bottom = pos.y + (bodySize / 2.0f);

		// Ÿ�ٰ��� �浹Ȯ��
		if (CheckCollision())
		{
			isFire = false;
			target->SetIsAlive(false);
		}

		// ȭ���� ������� Ȯ��
		if (shape.left > WIN_SIZE_X || shape.right < 0 ||
			shape.top > WIN_SIZE_Y || shape.bottom < 0)
		{
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
		img->Render(hdc, pos.x, pos.y);
		//Ellipse(hdc, shape.left, shape.top, shape.right, shape.bottom);
	}
}

void Ammo::Release()
{
}

bool Ammo::CheckCollision()
{
	// ��� �̻��ϰ� �� ��ũ�� �浹�ߴ��� �Ǵ��ұ�?

	if (!target)	return false;

	// �� ���� ��ǥ�� �Ÿ� ���
	POINTFLOAT ammoPos = pos;
	POINTFLOAT targetPos = target->GetPos();

	float distance = sqrtf((ammoPos.x - targetPos.x) * (ammoPos.x - targetPos.x)
		+ (ammoPos.y - targetPos.y) * (ammoPos.y - targetPos.y));

	// �������� �� ���
	float ammoRadius = bodySize / 2.0f;
	float targetRadius = target->GetBodySize() / 2.0f;

	float sum = ammoRadius + targetRadius;

	// �� (�������� ���� �� ũ�� �浹)
	if (distance < sum)
		return true;

	return false;
}

void Ammo::SetMoveDir(string dir)
{
	cout << dir;
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
