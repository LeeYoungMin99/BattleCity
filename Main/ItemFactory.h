#pragma once

class Item;
class ItemFactory
{
public:
	virtual Item* CreateTank() = 0;
};

class HelmetItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
};

class ClockItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
};

class ShovelItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
};

class StarItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
};

class GrenadeItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
};

class TankLifeItemFactory : public ItemFactory
{
public:
	virtual Item* CreateTank() override;
};