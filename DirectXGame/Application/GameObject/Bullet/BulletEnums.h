#pragma once
#include <stdint.h>
#include <string>
#include "Engine/GlobalVariables/GlobalVariables.h"

// 弾の種類
enum class BulletType : uint32_t
{
	kNormal,	// 通常
	kTracking,	// 追尾
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
	kStraight,
	kWave,
	kTracking,
};

struct TrackingData
{
	float trackFrame;
	float damping;
	float lerpRadius;
	float baseSpeed;
	float initSpeed;

	void LoadGlobalData(std::string groupName) {

		GlobalVariables* globalVariable = GlobalVariables::GetInstance();

		trackFrame = globalVariable->GetValue<float>(groupName, "TrackFrame");
		damping = globalVariable->GetValue<float>(groupName, "Damping");
		lerpRadius = globalVariable->GetValue<float>(groupName, "LerpRadius");
		baseSpeed = globalVariable->GetValue<float>(groupName, "BaseSpeed");
		initSpeed = globalVariable->GetValue<float>(groupName, "InitSpeed");

	}
};

// Descending : 降順
// Ascending : 昇順