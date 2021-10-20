#pragma once

class Item;
class ItemFactorial
{
public:
	virtual Item* CreateTank() = 0;
};

class HelmetItemFactorial : public ItemFactorial
{
public:
	virtual Item* CreateTank() override;
};

class ClockItemFactorial : public ItemFactorial
{
public:
	virtual Item* CreateTank() override;
};

class ShovelItemFactorial : public ItemFactorial
{
public:
	virtual Item* CreateTank() override;
};

class StarItemFactorial : public ItemFactorial
{
public:
	virtual Item* CreateTank() override;
};

class GrenadeItemFactorial : public ItemFactorial
{
public:
	virtual Item* CreateTank() override;
};

class TankLifeItemFactorial : public ItemFactorial
{
public:
	virtual Item* CreateTank() override;
};