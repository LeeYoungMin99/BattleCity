#pragma once
#include "Config.h"
#include "GameObject.h"
#include "Ammo.h"

class Tank : public GameObject
{
public:
	TankType type;
	MoveDir moveDir;
	int checkMoveCount = 0;
	int enforceCount = 0;
	bool isAlive;

	// 필요한 멤버변수인지 확인필요.
	POINTFLOAT barrelEnd;
	float barrelSize;
	float barrelAngle;

	int ammoCount;
	Ammo* ammoPack;

public:
	virtual HRESULT Init(TankType type) { return E_NOTIMPL; };	// 부모클래스의 함수 중 기능이 다른 경우는
	virtual void Update() {}									// 오버라이딩을 한다
	virtual void Render(HDC hdc) {}
	virtual void Release() {}

	virtual void Move(MoveDir dir) {};
	virtual void Fire() {};

	void AutoMove();
	void RotateBarrelAngle(float rotateAngle);


	inline void SetBarrelAngle(float angle) { this->barrelAngle = angle; }
	inline void SetIsAlive(bool alive) { this->isAlive = alive; }

	Tank();
	virtual ~Tank();
};

class PlayerTank : public Tank
{
public:
	virtual HRESULT Init(TankType type);
	virtual void Update();						
	virtual void Render(HDC hdc);
	virtual void Release();

	virtual void Move(MoveDir dir) override;
	virtual void Fire() override;
	void ProcessInputKey();

	PlayerTank() {}
	virtual ~PlayerTank() {}
};

class NormalEnemyTank : public Tank
{
public:
	virtual HRESULT Init(TankType type);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	virtual void Move(MoveDir dir) override;
	virtual void Fire() override {}
	void ProcessInputKey();

	NormalEnemyTank() {}
	virtual ~NormalEnemyTank() {}
};

class SpeedEnemyTank : public Tank
{
public:
	virtual HRESULT Init(TankType type);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	virtual void Move(MoveDir dir) override;
	virtual void Fire() override;
};

class RapidEnemyTank : public Tank
{
public:
	virtual HRESULT Init(TankType type);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	virtual void Move(MoveDir dir) override;
	virtual void Fire() override;

};

class EnemyTank : public Tank
{
public:
	virtual HRESULT Init(TankType type);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	virtual void Move(MoveDir dir) override;
	virtual void Fire() override;
};