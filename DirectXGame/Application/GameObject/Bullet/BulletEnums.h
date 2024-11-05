#pragma once
#include <stdint.h>
#include <string>
#include "Engine/GlobalVariables/GlobalVariables.h"

// 追尾の種類
enum class BulletType : uint32_t
{
	kNormal,	// 通常
	kTracking,	// 追尾
};
// 追尾の種類
enum class TrackingType : uint32_t
{
	kSuperior,	// 優等
	kInferior,	// 劣等
	kGenius,	// 秀才
};

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
		lerpRadius = globalVariable->GetValue<float>(groupName, "BaseSpeed");
		baseSpeed = globalVariable->GetValue<float>(groupName, "InitSpeed");
		initSpeed = globalVariable->GetValue<float>(groupName, "LerpRadius");

	}
};

// Descending : 降順
// Ascending : 昇順