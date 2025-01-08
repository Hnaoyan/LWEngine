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

// 追従の速度段階
enum class TrackingStep : uint32_t
{
	kSlow,
	kUpSpeed,
};

// 命中関係の値
struct AccuracyData
{
	float trackingAccuracy;	// 追従命中率の割合
	float propulsionDamping; // 推進力減衰率
	float forceDamping;	// 力に対しての減衰率
	float smoothFactor;	// 平滑化のずらし率

	float ratio;	// 全体を決める確率の割合
};

// 追従データ
struct TrackingData
{
	// 追従処理のパラメータ
	float trackFrame;	// 追従フレーム
	float damping;	// 減衰率
	float lerpRadius;	// 広がる半径
	float baseSpeed;	// 基本速度
	float initSpeed;	// 初速度

	//---ステート関係---//
	// 追従ステート
	float straightFrame;	// 直進の時間
	float trackingFrame;	// 追従している時間
	// 命中関係
	AccuracyData accuracy;
	// 波ステート
	float waveFrame;		// 波軌道の時間

	// 急旋回ステート
	float turnFrame;	// 旋回時間

	//---遷移種類---//
	int32_t transitionType = 0;	// 切り替わりの種類
	int32_t accuracyType = 0;	//0:命中,1:ちょい外れる,2:結構外れる

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

