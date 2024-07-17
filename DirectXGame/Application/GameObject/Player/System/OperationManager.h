#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "LockOn.h"
#include "AimManager.h"

class Player;
class Input;
class SampleBulletManager;

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
	void SetManager(SampleBulletManager* manager) { bulletManager_ = manager; }
	void SetEnemyList(std::vector<std::unique_ptr<SampleEnemy>>* lists) { lockOn_.SetEnemyList(lists); }
	AimManager* GetAimManager() { return &aimManager_; }
	LockOn* GetLockOn() { return &lockOn_; }
private: // POINTER
	// 親
	Player* player_ = nullptr;
	// 入力クラス
	Input* input_ = nullptr;
	// 弾の管理クラス
	SampleBulletManager* bulletManager_ = nullptr;

private: // USER	
	FrameTimer shotTimer_;
	// 多重クリック回避
	FrameTimer lockToggleTimer_;

private: // SYSTEM
	// ロックオンシステム
	LockOn lockOn_;
	// エイム関係
	AimManager aimManager_;

};