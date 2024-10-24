#pragma once
#include "Engine/Math/Vector/Vector3.h"
#include "Engine/3D/User/Trail3D.h"
#include "Engine/3D/Instancing/InstancedUnit.h"
#include <vector>
#include <optional>

class IBullet;
class ParticleEmitter;

class BulletTrail
{
public:
	// コンストラクタ
	BulletTrail();
	BulletTrail(IBullet* unit);
	~BulletTrail();
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

	// テクスチャ
	static uint32_t sTexture;

public:
	// ポリゴン
	std::unique_ptr<Trail3D> polygon_;
	// 長さ設定
	void SetLength(int32_t length) { maxLength = length; }
	// 弾
	void SetBullet(IBullet* bullet) { unit_ = bullet; }

	void SetMoveEmitter(ParticleEmitter* moveEmitter) { moveEmitter_ = moveEmitter; }
	ParticleEmitter* GetEmitter() { return moveEmitter_; }

	void SetBulletTag(const std::string& tag) { bulletTag_ = tag; }
	std::string GetTag() { return bulletTag_; }

	bool IsDelete() { return isDelete_; }
private:
	void GlobalValueInitialize();

private: // SYSTEM
	// 軌跡用の座標保存
	std::vector<Vector3> trailPoints_;
	// 保存できる座標数
	int32_t maxLength = 0;
	// 個体のポインタ
	std::optional<IBullet*> unit_ = std::nullopt;
	// 消すフラグ
	bool isDelete_ = false;
	// エミッター
	ParticleEmitter* moveEmitter_ = nullptr;

	std::string bulletTag_;

private: // USER
	// 消えるまでのタイマー
	FrameTimer deleteTimer_;
};
