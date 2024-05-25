#pragma once
#include "../Math/MathLib.h"
#include "../3D/CBuffer.h"
#include "../Camera/ICamera.h"

#include <wrl.h>
#include <d3d12.h>

class WorldTransform
{
public:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	CBufferWorldTransform* constMap_ = nullptr;

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
	Transform transform_ = { {1.0f,1.0f,1.0f} ,{},{} };
	// 親子ポインタ
	const WorldTransform* parent_ = nullptr;
	// ワールド行列
	Matrix4x4 worldMatrix_;
	// カメラのポインタ
	ICamera* camera_ = nullptr;
	bool isBillBoard_ = false;

};