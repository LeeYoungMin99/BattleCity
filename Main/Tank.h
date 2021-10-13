#pragma once
#include "Config.h"
#include "GameObject.h"
#include "Ammo.h"

class Tank : public GameObject
{
public:
	TankType type = TankType::Player;
	MoveDir moveDir = MoveDir::Up;
	int checkMoveCount = 0;
	int enforceCount = 2;
	bool bIsAlive = true;

	int ammoCount = 0;
	
	TILE_INFO* tileInfo = nullptr;
	Ammo* ammoPack = nullptr;

	int delay = rand();
	int elapsedCount = 0;
public:
	virtual HRESULT Init(TILE_INFO* tileInfo) { return E_NOTIMPL; };	// 부모클래스의 함수 중 기능이 다른 경우는
	virtual void Update();												// 오버라이딩을 한다
	virtual void Render(HDC hdc);
	virtual void Release();

	virtual void Move();
	virtual void Fire() {};

	virtual bool IsCollided();
	virtual void SetShape();

	inline void SetIsAlive(bool alive) { this->bIsAlive = alive; }

	Tank() {}
	virtual ~Tank() {}
};

class PlayerTank : public Tank
{
public:
	virtual HRESULT Init(TILE_INFO* tileInfo) override;
	virtual void Update() override;						
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	virtual void Move() override;
	virtual void Fire() override;

	PlayerTank() {}
	virtual ~PlayerTank() {}
};

class NormalEnemyTank : public Tank
{
public:
	virtual HRESULT Init() override;

	NormalEnemyTank() {}
	virtual ~NormalEnemyTank() {}
};

class SpeedEnemyTank : public Tank
{
public:
	virtual HRESULT Init() override;

	SpeedEnemyTank() {}
	virtual ~SpeedEnemyTank() {}
};

class RapidEnemyTank : public Tank
{
public:
	virtual HRESULT Init() override;

	RapidEnemyTank() {}
	virtual ~RapidEnemyTank() {}
};

class DefensiveEnemyTank : public Tank
{
public:
	virtual HRESULT Init() override;

	DefensiveEnemyTank() {}
	virtual ~DefensiveEnemyTank() {}
};