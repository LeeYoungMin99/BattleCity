#pragma once
#include "Config.h"
#include "GameObject.h"

enum class BulletDir { Left, Right, Up, Down };

class Image;
class Tank;		// ���漱�� (Tank��� Ŭ������ �ִ�)
class Ammo : public GameObject
{
private:
	//Tank* owner;					// 3. Ammo��ü�� �����ڸ� �����ͷ� 
									// ������� �Ҵ�

	//POINTFLOAT enemyPos;			// 1. �� ��ǥ�� ���� ��������� ������ �ִ´�.
	Tank* target;					// 2. �� ��ü�� �޸��ּҸ� ��������� ������ �ִ´�.

	bool isFire;
	//bool isAlive;

	float moveAngle;

	Image* img;

	float moveSpeed;

	BulletDir bulletDir;
public:
	HRESULT Init();		
	//void Init(Tank* tank);		// 1. Tank��ü�� �����͸� ����
	void Update();
	void Render(HDC hdc);
	void Release();

	bool CheckCollision();

	void SetMoveDir(string dir);


	// 2. Setter�� ����
	// this : this�� ȣ��� �Լ��� ȣ���� �ν��Ͻ�(�� �޸��ּ�)
	void SetIsFire(bool fire);
	inline bool GetIsFire() { return this->isFire; }

	inline void SetMoveAngle(float angle) { this->moveAngle = angle; }
	inline void SetTarget(Tank* target) { this->target = target; }

	//inline void SetIsAlive(bool alive) { this->isAlive = alive; }
	//inline bool GetIsAlive() { return this->isAlive; }
	Ammo();
	~Ammo();
};

