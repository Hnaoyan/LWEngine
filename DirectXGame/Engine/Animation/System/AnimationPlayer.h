#pragma once
#include "../Animation.h"

class AnimationPlayer
{
public:
	enum class PlayBack : uint32_t {
		kForward,
		kReverse,
		kStop,
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="data"></param>
	void Initialize(AnimationData* data);
	/// <summary>
	/// 再生方向設定
	/// </summary>
	/// <param name="type"></param>
	void SetPlayBackType(PlayBack type) { playType_ = type; }
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// imgui
	/// </summary>
	/// <param name="parentName"></param>
	void ImguiDraw(std::string parentName);

	bool isForward_ = true;
	bool isStop_ = false;
	AnimationData* animationData_ = nullptr;
	float animationTime_ = 0.0f;
	// 再生方向
	PlayBack playType_ = PlayBack::kForward;
};
