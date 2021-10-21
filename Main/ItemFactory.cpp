#include "ItemFactory.h"
#include "Item.h"

Item* HelmetItemFactory::CreateTank()
{
	return new HelmetItem;
}

Item* ClockItemFactory::CreateTank()
{
	return new ClockItem;
}

Item* ShovelItemFactory::CreateTank()
{
	return new ShovelItem;
}

Item* StarItemFactory::CreateTank()
{
	return new StarItem;
}

Item* GrenadeItemFactory::CreateTank()
{
	return new GrenadeItem;
}

Item* TankLifeItemFactory::CreateTank()
{
	return new TankLifeItem;
}
