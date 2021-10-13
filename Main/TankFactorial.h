#pragma once

class Tank;
class TankFactorial
{
public:
	virtual Tank* CreateTank() = 0;
};

class PlayerTankFactorial : public TankFactorial
{
public:
	virtual Tank* CreateTank() override;
};

class NormalEnemyTankFactorial : public TankFactorial
{
public:
	virtual Tank* CreateTank() override;
};

class SpeedEnemyTankFactorial : public TankFactorial
{
public:
	virtual Tank* CreateTank() override;
};

class RapidEnemyTankFactorial : public TankFactorial
{
public:
	virtual Tank* CreateTank() override;
};

class DefensiveEnemyTankFactorial : public TankFactorial
{
public:
	virtual Tank* CreateTank() override;
};