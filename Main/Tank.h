#pragma once
#include "Config.h"
#include "GameObject.h"
#include "Ammo.h"

class EnemyManager;
class ItemManager;
class Item;
class Tank : public GameObject
{
public:
	int HP = 0;

	Image* spawnImg = nullptr;
	Image* itemTank = nullptr;
	float spawnTime = 2.0f;
	float spawnElapsedCount = 0.0f;
	int spawnImgFrame = 0;
	int maxSpawnImgFrame = 3;
	bool bReverseSpawnImg = false;
	bool bCheckSpawnStatus = true;
	bool bCheckSpawnCollided = false;

	TankType type = TankType::Player;
	MoveDir moveDir = MoveDir::Up;
	int checkMoveCount = 0;
	int enforceCount = 0;
	bool bIsAlive = true;
	
	vector<Item*> itemList = {};
	vector<Item*>::iterator itItemList = {};

	int ammoCount = 0;

	POINTFLOAT BarrelPos;

	TILE_INFO* tileInfo = nullptr;
	Tank* playerTank = nullptr;
	EnemyManager* enemyMgr = nullptr;
	vector<Tank*> enemyTanks = {};
	vector<Tank*>::iterator itEnemyTanks = {};
	Ammo* ammoPack = nullptr;

	ItemManager* itemManager;
	Item* item;

	float delay = RANDOM(1, 3);
	float elapsedCount = 0.0f;

	float delay_2 = RANDOM(10,15);	//노말탱크 공격딜레이
	int testelapsed;				//노말탱크 공격딜레이
	int testelapsed_2;		//아이템탱크 번쩍번쩍
	int checkMoveCount_2;	//아이템탱크 번쩍번쩍
	bool bItem = false;		//아이템 여부

public:
	virtual HRESULT Init(TILE_INFO* tileInfo, EnemyManager* enemyMgr, Tank* playerTank = nullptr, vector<Item*> item = {}) { return E_NOTIMPL; };	// 부모클래스의 함수 중 기능이 다른 경우는
	virtual void Update();												// 오버라이딩을 한다
	virtual void Render(HDC hdc);
	virtual void Release();

	virtual void Move();
	virtual void Fire();

	virtual bool IsCollided();
	virtual void SetShape();
	
	inline virtual void increaseScore() = 0;

	inline void SetIsAlive(bool alive) { this->bIsAlive = alive; }


	Tank() {}
	virtual ~Tank() {}
};

class PlayerTank : public Tank
{
private:
	Image* shieldImg = nullptr;
	float shieldTime = 3.0f;
	float shieldElapsedCount = 0.0f;
	bool bCheckShieldOn = false;
	bool bShieldImageChanged = false;

public:
	virtual HRESULT Init(TILE_INFO* tileInfo, EnemyManager* enemyMgr, Tank* playerTank = nullptr, vector<Item*> item = {}) override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	virtual void Move() override;
	virtual void Fire() override;
	inline virtual void increaseScore() override { };

	PlayerTank() {}
	virtual ~PlayerTank() {}
};

class NormalEnemyTank : public Tank
{
public:
	virtual HRESULT Init(TILE_INFO* tileInfo, EnemyManager* enemyMgr, Tank* playerTank = nullptr, vector<Item*> item = {}) override;
	
	virtual void Fire() override;
	inline virtual void increaseScore() override { GameManager::GetSingleton()->defeatNormalTank++; } ;

	NormalEnemyTank() {}
	virtual ~NormalEnemyTank() {}
};

class SpeedEnemyTank : public Tank
{
public:
	virtual HRESULT Init(TILE_INFO* tileInfo, EnemyManager* enemyMgr, Tank* playerTank = nullptr, vector<Item*> item = {}) override;

	virtual void Fire() override;

	inline virtual void increaseScore() override { GameManager::GetSingleton()->defeatNormalTank++; };
	SpeedEnemyTank() {}
	virtual ~SpeedEnemyTank() {}
};

class RapidEnemyTank : public Tank
{
public:
	virtual HRESULT Init(TILE_INFO* tileInfo, EnemyManager* enemyMgr, Tank* playerTank = nullptr, vector<Item*> item = {}) override;

	virtual void Fire() override;
	inline virtual void increaseScore() override { GameManager::GetSingleton()->defeatNormalTank++; };

	RapidEnemyTank() {}
	virtual ~RapidEnemyTank() {}
};

class DefensiveEnemyTank : public Tank
{
public:
	virtual HRESULT Init(TILE_INFO* tileInfo, EnemyManager* enemyMgr, Tank* playerTank = nullptr, vector<Item*> item = {}) override;

	virtual void Fire() override;
	inline virtual void increaseScore() override { GameManager::GetSingleton()->defeatNormalTank++; };

	DefensiveEnemyTank() {}
	virtual ~DefensiveEnemyTank() {}
};