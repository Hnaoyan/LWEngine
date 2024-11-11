#pragma once
#include "Engine/Base/CBufferCommon.h"

class InstancedUnit
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

public: // アクセッサ
	// 死亡フラグ
	bool IsDead() { return isDead_; }
	bool IsBillBoard() { return isBillBoard_; }
	// ワールドトランスフォーム
	Matrix4x4 GetWorldMatrix() { return worldMatrix_; }
	// ワールド座標
	Vector3 GetWorldPosition() { return Vector3(worldMatrix_.m[3][0], worldMatrix_.m[3][1], worldMatrix_.m[3][2]); }
	// 親の設定
	void SetParent(Matrix4x4* parent) { parent_ = parent; }
public:
	EulerTransform transform_;

protected:
	Matrix4x4 worldMatrix_;
	Matrix4x4 localMatrix_;
	const Matrix4x4* parent_ = nullptr;
	bool isDead_ = false;
	bool isBillBoard_ = false;
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
};
