#pragma once

class Item;
class ItemFactory
{
public:
	virtual Item* CreateTank() = 0;
	virtual ~ItemFactory() = default;
};

class HelmetItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
	virtual ~HelmetItemFactory() = default;
};

class ClockItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
	virtual ~ClockItemFactory() = default;
};

class ShovelItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
	virtual ~ShovelItemFactory() = default;
};

class StarItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
	virtual ~StarItemFactory() = default;
};

class GrenadeItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
	virtual ~GrenadeItemFactory() = default;
};

class TankLifeItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
	virtual ~TankLifeItemFactory() = default;
};