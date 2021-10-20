#include "ItemFactorial.h"
#include "Item.h"

Item* HelmetItemFactorial::CreateTank()
{
	return new HelmetItem;
}

Item* ClockItemFactorial::CreateTank()
{
	return new ClockItem;
}

Item* ShovelItemFactorial::CreateTank()
{
	return new ShovelItem;
}

Item* StarItemFactorial::CreateTank()
{
	return new StarItem;
}

Item* GrenadeItemFactorial::CreateTank()
{
	return new GrenadeItem;
}

Item* TankLifeItemFactorial::CreateTank()
{
	return new TankLifeItem;
}
