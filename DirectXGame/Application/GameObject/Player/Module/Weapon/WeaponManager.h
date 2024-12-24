#pragma once
#include "Gear/IWeapon.h"
#include <memory>

class Player;

class WeaponManager
{
public:
	WeaponManager() {};
	WeaponManager(Player* player) : player_(player) {}
	~WeaponManager() = default;

public:
	void Initialize();
	void Update();

public:
	Player* player_ = nullptr;
	std::unique_ptr<IWeapon> weapon_;

};
