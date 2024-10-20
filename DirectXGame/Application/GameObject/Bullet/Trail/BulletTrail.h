#pragma once
#include "Engine/Math/Vector/Vector3.h"
#include "Engine/3D/User/Trail3D.h"
#include "Engine/3D/Instancing/InstancedUnit.h"
#include <vector>
#include <optional>

class IBullet;

class BulletTrail
{
public:
	// コンストラクタ
	BulletTrail();
	BulletTrail(IBullet* unit);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="newPoint"></param>
	void UpdateTrail(const Vector3& newPoint);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Update(ICamera* camera);

public:
	// 軌跡用の座標保存
	std::vector<Vector3> trailPoints_;
	// ポリゴン
	std::unique_ptr<Trail3D> polygon_;
	// 長さ設定
	void SetLength(int32_t length) { maxLength = length; }
	// 弾
	void SetBullet(IBullet* bullet) { unit_ = bullet; }
	bool IsDelete() { return isDelete_; }
private:
	void GlobalValueInitialize();

private:
	// 保存できる座標数
	int32_t maxLength = 0;
	// 個体のポインタ
	std::optional<IBullet*> unit_ = std::nullopt;
	// 消すフラグ
	bool isDelete_ = false;

	FrameTimer deleteTimer_;

};
