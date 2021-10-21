#include "TankFactory.h"
#include "Tank.h"

Tank* PlayerTankFactory::CreateTank()
{
	return new PlayerTank;
}

Tank* NormalEnemyTankFactory::CreateTank()
{
	return new NormalEnemyTank;
}

Tank* SpeedEnemyTankFactory::CreateTank()
{
	return new SpeedEnemyTank;
}

Tank* RapidEnemyTankFactory::CreateTank()
{
	return new RapidEnemyTank;
}

Tank* DefensiveEnemyTankFactory::CreateTank()
{
	return new DefensiveEnemyTank;
}
