#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Aim/LockOn.h"
#include "Aim/AimManager.h"

class Player;
class Input;
class BulletManager;

class OparationManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
private:
	/// <summary>
	/// 入力更新
	/// </summary>
	void InputUpdate();

public:
	void SetManager(BulletManager* manager) { bulletManager_ = manager; }
	void SetEnemyList(std::vector<std::unique_ptr<SampleEnemy>>* lists) { lockOn_.SetEnemyList(lists); }
	AimManager* GetAimManager() { return &aimManager_; }
	LockOn* GetLockOn() { return &lockOn_; }
private: // POINTER
	// 親
	Player* player_ = nullptr;
	// 入力クラス
	Input* input_ = nullptr;
	// 弾の管理クラス
	BulletManager* bulletManager_ = nullptr;

private: // USER
	bool isDash_ = false;
	int resetTime_ = 0;
	
	FrameTimer shotTimer_;
	FrameTimer lockOnCooltime_;

	Vector3 dashVelocity_{};
	FrameTimer dashCooltime_;

private: // SYSTEM
	// ロックオンシステム
	LockOn lockOn_;
	// エイム関係
	AimManager aimManager_;

};