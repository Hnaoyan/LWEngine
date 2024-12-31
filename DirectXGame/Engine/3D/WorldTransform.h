#pragma once
#include "../Math/MathLib.h"
#include "../Camera/Core/ICamera.h"
#include "Engine/Base/CBufferCommon.h"

#include <wrl.h>
#include <d3d12.h>

/// <summary>
/// ワールドトランスフォームクラス
/// </summary>
class WorldTransform
{
private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	CBufferWorldTransform* constMap_ = nullptr;
public:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> GetCBuffer() { return constBuffer_; }
	CBufferWorldTransform* GetCMap() { return constMap_; }
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const{ return { worldMatrix_.m[3][0],worldMatrix_.m[3][1] ,worldMatrix_.m[3][2] }; }

	void SetModelData(const Matrix4x4& local) {
		localMatrix_ = local;
	}

private:
	/// <summary>
	/// 定数バッファ作成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピング
	/// </summary>
	void Map();
public:
	/// <summary>
	/// バッファに転送
	/// </summary>
	void TransferMatrix();

public:
	// トランスフォーム
	EulerTransform transform_ = { {1.0f,1.0f,1.0f} ,{},{} };
	// 方向ベクトル
	Vector3 rotateDirect_ = {};
	// 親子ポインタ
	const WorldTransform* parent_ = nullptr;
	// 行列のみで渡す親
	const Matrix4x4* parentMatrix_ = nullptr;
	// ワールド行列
	Matrix4x4 worldMatrix_ = {};
	// ローカル行列
	Matrix4x4 localMatrix_ = {};

	// カメラのポインタ
	ICamera* camera_ = nullptr;
	// ビルボードを行うか
	bool isBillBoard_ = false;
	// ベクトルでの回転行列の作成
	bool isVectorRotate_ = false;

	// クォータニオンで計算するかどうか
	bool isQuaternion_ = false;

	// 回転用のクォータニオン
	Quaternion rotateQuaternion_ = {};

	// 軸
	Vector3 axis_ = { 0.0f,0.0f,1.0f };
	// 回転ベクトル
	Vector3 rotVector_ = { 0.0f,0.0f,0.0f };
	// 角度
	float angle_ = 0.0f;

};