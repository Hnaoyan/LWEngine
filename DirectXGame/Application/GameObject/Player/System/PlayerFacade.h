#pragma once
#include "PlayerSystemLists.h"

class Player;

class PlayerFacade
{
public:
	void Initialize(Player* player);

private:
	// Hp関係
	PlayerContext::HealthManager healthManager_;
	// パーティクル
	PlayerContext::ParticleManager particleManager_;
	// エネルギー
	PlayerContext::EnergyManager energyManager_;

};
