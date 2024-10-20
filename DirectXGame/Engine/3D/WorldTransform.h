#pragma once
#include "../Math/MathLib.h"
#include "../Camera/Core/ICamera.h"
#include "Engine/Base/CBufferCommon.h"

#include <wrl.h>
#include <d3d12.h>

class WorldTransform
{
private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	CBufferWorldTransform* constMap_ = nullptr;
	//ConstantBufferMapContext<CBufferWorldTransform> data_;
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
	/// バッファに転送
	/// </summary>
	void TransferMatrix();
	/// <summary>
	/// マッピング
	/// </summary>
	void Map();

public:
	// トランスフォーム
	EulerTransform transform_ = { {1.0f,1.0f,1.0f} ,{},{} };
	// 親子ポインタ
	const WorldTransform* parent_ = nullptr;
	// 行列のみで渡す親
	const Matrix4x4* parentMatrix_ = nullptr;
	// ワールド行列
	Matrix4x4 worldMatrix_ = {};
	// ローカル行列
	Matrix4x4 localMatrix_ = {};
	// 方向ベクトル
	Vector3 rotateDirect_;

	// カメラのポインタ
	ICamera* camera_ = nullptr;
	bool isBillBoard_ = false;
	// ベクトルでの回転行列の作成
	bool isVectorRotate_ = false;

	bool isQuaternion_ = false;

	Quaternion rotateQuaternion_;

	Vector3 axis_{ 0.0f,0.0f,1.0f };
	Vector3 rotVector_{ 0.0f,0.0f,0.0f };
	float angle_ = 0.0f;

};