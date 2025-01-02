#pragma once
#include <stdint.h>
#include <string>
#include "Engine/GlobalVariables/GlobalVariables.h"

// 弾の種類
enum class BulletType : uint32_t
{
	kNormal,	// 通常弾
	kTracking,	// 追従弾
	kContainer,	// ミサイルコンテナ弾
};
// 追従先の属性（座標の決め方）
enum class TrackingAttribute : uint32_t
{
	kSuperior,	// 優等
	kInferior,	// 劣等
	kGenius,	// 秀才
	kNone,		// どれでもない
	kMaxSize,	// 最大値
};
// 追従の状態
enum class TrackingState : uint32_t
{
	kStraight,	// 直進
	kWave,		// 波
	kTracking,	// 追従
	kTurnToTarget,	// 急旋回
};

struct TrackingData
{
	float trackFrame;	// 追従フレーム
	float damping;	// 減衰率
	float lerpRadius;	// 広がる半径
	float baseSpeed;	// 基本速度
	float initSpeed;	// 初速度

	//---ステート関係---//
	float straightFrame;	// 直進の時間
	float waveFrame;		// 波軌道の時間
	float trackingFrame;	// 追従している時間

	/// <summary>
	/// 読み込み関数
	/// </summary>
	/// <param name="groupName"></param>
	void LoadGlobalData(std::string groupName)
	{
		GlobalVariables* globalVariable = GlobalVariables::GetInstance();
		damping = globalVariable->GetValue<float>(groupName, "Damping");
		lerpRadius = globalVariable->GetValue<float>(groupName, "LerpRadius");
		baseSpeed = globalVariable->GetValue<float>(groupName, "BaseSpeed");
		initSpeed = globalVariable->GetValue<float>(groupName, "InitSpeed");
		// タイマー関係
		trackFrame = globalVariable->GetValue<float>(groupName, "TrackFrame");
		straightFrame = globalVariable->GetValue<float>(groupName, "StraightFrame");
		
	}
};
