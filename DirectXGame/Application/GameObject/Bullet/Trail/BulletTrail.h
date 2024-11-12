#pragma once
#include "Engine/Math/Vector/Vector3.h"
#include "Engine/3D/User/Trail3D.h"
#include "Engine/3D/Instancing/InstancedUnit.h"
#include "Application/GameObject/Bullet/BulletEnums.h"
#include <vector>
#include <optional>

class IBullet;
class GPUParticleEmitter;

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

	void SetMoveEmitter(GPUParticleEmitter* moveEmitter) { moveEmitter_ = moveEmitter; }
	GPUParticleEmitter* GetEmitter() { return moveEmitter_; }

	void SetBulletTag(const std::string& tag) { bulletTag_ = tag; }
	std::string GetTag() { return bulletTag_; }

	bool IsDelete() { return isDelete_; }

	// 隠すフラグ
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; }
	bool IsInvisible() { return isInvisible_; }

	Vector3 GetBeginPoint() {
		auto endPoint = trailPoints_.end();
		endPoint--;
		return *endPoint;
	}

	void SetTrailColor(const Vector3& color) { polygon_->SetColor(Vector3(color)); }
	void SetAttribute(TrackingAttribute type) { attribute_ = type; }
	TrackingAttribute GetAttribute() { return attribute_; }
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
	// 描画しないフラグ
	bool isInvisible_ = false;
	// エミッター
	GPUParticleEmitter* moveEmitter_ = nullptr;

	// 
	TrackingAttribute attribute_;

	std::string bulletTag_;

private: // USER
	// 消えるまでのタイマー
	FrameTimer deleteTimer_;
};
