#include "TankFactorial.h"
#include "Tank.h"

Tank* PlayerTankFactorial::CreateTank()
{
	return new PlayerTank;
}

Tank* NormalEnemyTankFactorial::CreateTank()
{
	return new NormalEnemyTank;
}

Tank* SpeedEnemyTankFactorial::CreateTank()
{
	return new SpeedEnemyTank;
}

Tank* RapidEnemyTankFactorial::CreateTank()
{
	return new RapidEnemyTank;
}

Tank* DefensiveEnemyTankFactorial::CreateTank()
{
	return new DefensiveEnemyTank;
}
