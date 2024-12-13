#pragma once
#include "Engine/Math/Vector/Vector3.h"
#include "Engine/3D/User/Trail3D.h"
#include "Engine/3D/Instancing/InstancedUnit.h"
#include "Application/GameObject/Bullet/BulletEnums.h"
#include <vector>
#include <optional>

class IBullet;
class GPUParticleEmitter;

/// <summary>
/// 弾の軌跡クラス
/// </summary>
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
public: // アクセッサ
	// 長さ設定
	void SetLength(int32_t length) { maxLength = length; }
	// 弾
	void SetBullet(IBullet* bullet) { unit_ = bullet; }

	void SetMoveEmitter(GPUParticleEmitter* moveEmitter) { moveEmitter_ = moveEmitter; }
	GPUParticleEmitter* GetEmitter() { return moveEmitter_; }

	void SetBulletTag(const std::string& tag) { bulletTag_ = tag; }

	std::string GetTag() const { return bulletTag_; }
	bool IsDelete() const { return isDelete_; }

	// 隠すフラグ
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; }
	bool IsInvisible() const { return isInvisible_; }

	Vector3 GetBeginPoint() {
		Vector3 result = {};
		if (trailPoints_.size() > 0) {
			auto point = --trailPoints_.end();
			result = (*point);
		}
		return result;
	}

	void SetTrailColor(const Vector3& color) { polygon_->SetColor(Vector3(color)); }
	void SetAttribute(TrackingAttribute type) { attribute_ = type; }
	TrackingAttribute GetAttribute() const { return attribute_; }
private:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue();

private: // SYSTEM
	// 軌跡用の座標保存
	std::vector<Vector3> trailPoints_;
	// 保存できる座標数
	int32_t maxLength = 0;
	// 追従属性
	TrackingAttribute attribute_;
	// 個体のポインタ
	std::optional<IBullet*> unit_ = std::nullopt;
	// エミッター
	GPUParticleEmitter* moveEmitter_ = nullptr;
	// 弾のタグ
	std::string bulletTag_;

	// 消すフラグ
	bool isDelete_ = false;
	// 描画しないフラグ
	bool isInvisible_ = false;

private: // USER
	// 消えるまでのタイマー
	FrameTimer deleteTimer_;

};
