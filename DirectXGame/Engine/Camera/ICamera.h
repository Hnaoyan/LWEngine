#pragma once
#include "../3D/CBuffer.h"
#include "../WindowAPI/WindowAPI.h"
#include <wrl.h>
#include <d3d12.h>
#include <numbers>

class ICamera
{
public:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング用
	CBufferDataCamera* constMap_ = nullptr;

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
	void ImGuiDraw();

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
	/// <summary>
	/// マップに送る処理
	/// </summary>
	void TransferMatrix();

protected:
	// ビュー行列
	Matrix4x4 viewMatrix_;
	// プロジェクション行列
	Matrix4x4 projectionMatrix_;

public:
	// 
	Transform transform_ = {};
	// 視野角の値
	float fov_ = 45.0f;
	// 視野角の角度
	float fovAngle_ = fov_ * (float)(std::numbers::pi / 180.0);
	// アスペクト比率
	float aspectRatio_ = (float)WindowAPI::kWindowWidth / (float)WindowAPI::kWindowHeight;

	float nearZ = 0.1f;
	float farZ = 1000.0f;
};
