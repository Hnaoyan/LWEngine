#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "../../WindowAPI/WindowAPI.h"
#include <wrl.h>
#include <d3d12.h>
#include <numbers>

class ICamera
{
private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング用
	CBufferDataCamera* constMap_ = nullptr;
public: // 定数バッファのアクセッサ
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> GetCBuffer() { return constBuff_; }
	// マップ
	CBufferDataCamera* GetView() { return constMap_; }

public:	// 継承できるように
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() { UpdateMatrix(); };

	/// <summary>
	/// ImGui
	/// </summary>
	virtual void ImGuiDraw();

protected: // 共通で使う関数
	/// <summary>
	/// バッファーにマッピング
	/// </summary>
	void Map();
	/// <summary>
	/// バッファー作成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// 行列更新
	/// </summary>
	void UpdateMatrix();
public:
	/// <summary>
	/// マップに送る処理
	/// </summary>
	void TransferMatrix();

protected:
	// 視野角の値
	float fov_ = 45.0f;

public:
	// ビュー行列
	Matrix4x4 viewMatrix_ = {};
	// プロジェクション行列
	Matrix4x4 projectionMatrix_ = {};
	// 
	EulerTransform transform_ = {};

	const Vector3 kFrontVector = { 0.0f,0.0f,1.0f };

	Vector3 frontVector_ = {};
public:
	// ワールド座標
	Vector3 GetWorldPosition() { return Vector3(viewMatrix_.m[2][0], viewMatrix_.m[2][1], viewMatrix_.m[2][2]); }

private:
	// 視野角の角度
	float fovAngle_ = fov_ * (float)(std::numbers::pi / 180.0);
	// アスペクト比率
	float aspectRatio_ = (float)WindowAPI::kWindowWidth / (float)WindowAPI::kWindowHeight;

	float nearZ = 0.1f;
	float farZ = 1000.0f;
};
