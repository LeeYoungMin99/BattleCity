#pragma once
#include "Config.h"
#include "GameObject.h"
#include "Ammo.h"

class AmmoManager;
class EnemyManager;
class ItemManager;
class Item;
class Tank : public GameObject
{
protected:
	int HP = 1;
	Image* spawnImg = nullptr;
	Image* itemTank = nullptr;
	float spawnTime = 2.0f;
	float spawnElapsedCount = 0.0f;
	int spawnImgFrame = 0;
	int maxSpawnImgFrame = 3;
	bool bReverseSpawnImg = false;
	bool bCheckSpawnStatus = true;
	bool bCheckSpawnCollided = false;

	bool bIsAlive = true;
	TankType type = TankType::Player;
	MoveDir moveDir = MoveDir::Up;
	MoveDir previousDir = moveDir;
	int enforceCount = 0;
	

	int checkMoveCount = 0;
	int currFireNumberOfAmmo = 0;
	POINTFLOAT barrelPos = {};

	bool bCheckTankCollider;

	AmmoManager* ammoManager = nullptr;
	AmmoManager* targetAmmoManager = nullptr;
	GameEntity* stageInfo = nullptr;
	TILE_INFO* tileInfo = nullptr;
	Tank* playerTank = nullptr;
	vector<Tank*>* enemyTanks = nullptr;
	vector<Tank*>::iterator itEnemyTanks = {};
	ItemManager* itemManager;
	vector<Item*>* ItemList;
	vector<Item*>::iterator itItemList = {};

	float delay = RANDOM(1, 3);
	float elapsedCount = 0.0f;
	float delay_2 = RANDOM(10, 15);	//탱크 공격딜레이
	int testelapsed;				//탱크 공격딜레이
	int testelapsed_2;				//아이템탱크 번쩍번쩍
	int checkMoveCount_2;			//아이템탱크 번쩍번쩍
	bool bHaveItem = false;			//아이템 여부
	bool clockItem = false;

public:
	virtual HRESULT Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tileInfo,
		vector<Tank*>* enemyTanks, Tank* playerTank = nullptr, vector<Item*>* item = nullptr, GameEntity* stageInfo = nullptr)
	{
		return E_NOTIMPL;
	};

	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	virtual void Move();
	virtual void Fire();

	virtual void CorrectionPosX();
	virtual void CorrectionPosY();

	virtual bool IsCollided();
	virtual void SetShape();
	

	virtual void SpawnCollided();
	virtual void Action();
	void SpwanAnimation();
	void FlashItemTank();

	inline virtual void increaseScore() = 0;

	inline void SubtractHP(int damage) { this->HP -= damage; }
	inline void SubtractCurrFireNumberOfAmmo(int a) { this->currFireNumberOfAmmo -= a; }
	inline void AddEnforceCount(int a) { this->enforceCount += a; }

	inline void SetIsAlive(bool alive) { this->bIsAlive = alive; }
	inline void SetClockItem(bool clockItem) { this->clockItem = clockItem; }
	inline void SetBarrelPos(POINTFLOAT barrelPos) { this->barrelPos = barrelPos; }
	inline void SetElapsedCount(float f) { this->elapsedCount = f; }
	inline void SetHaveItem(bool b) { this->bHaveItem = b; }

	inline int GetHP() { return this->HP; }
	inline int GetEnforceCount() { return this->enforceCount; }
	inline bool GetCheckSpawnStatus() { return this->bCheckSpawnStatus; }
	inline bool GetHaveItem() { return this->bHaveItem; }
	inline MoveDir GetMoveDir() { return this->moveDir; }
	inline TankType GetType() { return this->type; }
	inline AmmoManager* GetTargetAmmoManager() { return this->targetAmmoManager; }
	inline POINTFLOAT GetBarrelPos() { return this->barrelPos; }
	inline void SetPlayerTank(Tank* player) { this->playerTank = player; }

	void CheckItem();

	Tank() {};
	virtual ~Tank() {}
};

class PlayerTank : public Tank
{
private:
	Image* shieldImg = nullptr;
	float shieldTime = 3.0f;
	float shieldElapsedCount = 0.0f;
	bool bShieldImageChanged = false;
	bool bCheckShieldOn = false;

public:
	virtual HRESULT Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tileInfo, vector<Tank*>* enemyTanks, Tank* playerTank = nullptr, vector<Item*>* item = nullptr, GameEntity* stageInfo = nullptr) override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	virtual void Fire() override;
	virtual void Action() override;

	void SpwanAndShieldAnimation();

	inline virtual void increaseScore() override { };

	inline void SetCheckShieldOn(bool b) { this->bCheckShieldOn = b; }
	inline bool GetCheckShieldOn() { return this->bCheckShieldOn; }

	PlayerTank();
	virtual ~PlayerTank() {}
};

class NormalEnemyTank : public Tank
{
public:
	virtual HRESULT Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tileInfo, vector<Tank*>* enemyTanks, Tank* playerTank = nullptr, vector<Item*>* item = nullptr, GameEntity* stageInfo = nullptr) override;

	inline virtual void increaseScore() override { GameManager::GetSingleton()->defeatNormalTank++; };

	NormalEnemyTank() {}
	virtual ~NormalEnemyTank() {}
};

class SpeedEnemyTank : public Tank
{
public:
	virtual HRESULT Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tileInfo, vector<Tank*>* enemyTanks, Tank* playerTank = nullptr, vector<Item*>* item = nullptr, GameEntity* stageInfo = nullptr) override;

	inline virtual void increaseScore() override { GameManager::GetSingleton()->defeatSpeedTank++; };

	SpeedEnemyTank() {}
	virtual ~SpeedEnemyTank() {}
};

class RapidEnemyTank : public Tank
{
public:
	virtual HRESULT Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tileInfo, vector<Tank*>* enemyTanks, Tank* playerTank = nullptr, vector<Item*>* item = nullptr, GameEntity* stageInfo = nullptr) override;

	inline virtual void increaseScore() override { GameManager::GetSingleton()->defeatRapidTank++; };

	RapidEnemyTank() {}
	virtual ~RapidEnemyTank() {}
};

class DefensiveEnemyTank : public Tank
{
public:
	virtual HRESULT Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tileInfo, vector<Tank*>* enemyTanks, Tank* playerTank = nullptr, vector<Item*>* item = nullptr, GameEntity* stageInfo = nullptr) override;

	inline virtual void increaseScore() override { GameManager::GetSingleton()->defeatDefensiveTank++; };

	DefensiveEnemyTank() {}
	virtual ~DefensiveEnemyTank() {}
};