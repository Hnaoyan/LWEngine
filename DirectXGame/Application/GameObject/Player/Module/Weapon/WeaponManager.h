#pragma once
#include "Gear/IWeapon.h"
#include "../../System/PlayerSystemLists.h"
#include <unordered_map>
#include <memory>

class Player;

class WeaponManager
{
public:
	WeaponManager() {};
	WeaponManager(Player* player) : player_(player) {}
	~WeaponManager() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 攻撃の受付（引数で種類決める形にする？
	/// </summary>
	void AttackReception();
	void LockOnReception();

public:
	PlayerContext::AimManager* GetAimManager() { return &aimManager_; }
	PlayerContext::LockOn* GetLockOn() { return &lockOn_; }

public:
	Player* player_ = nullptr;
	// 武器のリスト
	std::unordered_map<std::string, std::unique_ptr<IWeapon>> weapons_;
	std::unique_ptr<IWeapon> weapon_;

private:
	PlayerContext::AimManager aimManager_;
	PlayerContext::LockOn lockOn_;
	FrameTimer shotDuration_;
	FrameTimer lockOnDuration_;
};
