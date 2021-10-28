#pragma once

class Tank;
class TankFactory // 인터페이스 역할은 앞에 I를 붙여주면 좋음
{
public:
	virtual Tank* CreateTank() = 0;
	virtual ~TankFactory() = default;
};

class PlayerTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
	virtual ~PlayerTankFactory() = default;
};

class NormalEnemyTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
	virtual ~NormalEnemyTankFactory() = default;
};

class SpeedEnemyTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
	virtual ~SpeedEnemyTankFactory() = default;
};

class RapidEnemyTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
	virtual ~RapidEnemyTankFactory() = default;
};

class DefensiveEnemyTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
	virtual ~DefensiveEnemyTankFactory() = default;
};