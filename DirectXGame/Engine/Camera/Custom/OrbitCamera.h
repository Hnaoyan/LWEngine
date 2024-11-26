#pragma once
#include "../Core/ICamera.h"

class OrbitCamera : public ICamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
public:
	void SetObject(Vector3* start, Vector3* end) {
		objects_.first = start;
		objects_.second = end;
	}

private:
	// 回転の中心になる座標
	Vector3 centerPoint_{};
	// 回転用のクォータニオン
	Quaternion rotateQuaternion_{};
	// 2つのオブジェクト
	std::pair<Vector3*, Vector3*> objects_;
	// 開く方向
	Vector3 rotVector_ = Vector3::Right();
	float rotRatio_ = 0.5f;
	float focusT_ = 0.5f;
	// 向ける処理
	void FocusUpdate();

};

