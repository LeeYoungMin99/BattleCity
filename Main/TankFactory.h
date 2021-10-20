#pragma once

class Tank;
class TankFactory
{
public:
	virtual Tank* CreateTank() = 0;
};

class PlayerTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
};

class NormalEnemyTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
};

class SpeedEnemyTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
};

class RapidEnemyTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
};

class DefensiveEnemyTankFactory : public TankFactory
{
public:
	virtual Tank* CreateTank() override;
};