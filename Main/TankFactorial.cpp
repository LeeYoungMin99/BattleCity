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
