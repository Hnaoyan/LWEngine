#pragma once
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/Math/MathLib.h"
#include "Engine/3D/Instancing/InstancedGroup.h"

#include "Application/GameObject/Bullet/Trail/BulletTrail.h"
#include "Application/GameObject/Bullet/IBullet.h"
#include "../BossSystemStructs.h"

#include <memory>

namespace BossSystemContext
{
	class TrackingBullet : public IBullet {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="model"></param>
		void Initialize() override;
		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;
		/// <summary>
		/// ImGui描画
		/// </summary>
		/// <param name="name"></param>
		void ImGuiDraw() override;
		/// <summary>
		/// コールバック関数
		/// </summary>
		/// <param name="object"></param>
		void OnCollision(ColliderObject object) override;

		void SetBossPtr(Boss* boss) { boss_ = boss; }

	public: // アクセッサ
		EulerTransform GetTransform() { return transform_; }
		Vector3 GetVelocity() { return velocity_; }
		void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
		void SetPlayer(Player* player) { player_ = player; }
		void SetTrackType(TrackType type) { trackType_ = type; }

	private:
		// 追跡している時間
		FrameTimer trackTimer_;
		// プレイヤーのポインタ
		Player* player_ = nullptr;
		Boss* boss_ = nullptr;

		TrackType trackType_;

		float acceleration_ = 0.2f;

		float lerpRadius_ = 0.0f;

	private:
		// 通常
		void CalcStandardMissile();
		// 劣等
		void CalcInferiorMissile();
		// 優等
		void CalcSuperiorMissile();

	public: // 共通
		static float sTrackingFrame;
		static float sDamping;
		static float sLerpRadius;
		static float sBulletSpeed;
		static float sInitSpeed;

		//static float s

	};

	class NormalBullet : public IBullet {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="model"></param>
		void Initialize() override;
		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;
		/// <summary>
		/// ImGui描画
		/// </summary>
		/// <param name="name"></param>
		void ImGuiDraw() override;
		/// <summary>
		/// コールバック関数
		/// </summary>
		/// <param name="object"></param>
		void OnCollision(ColliderObject object) override;
	public: // アクセッサ
		EulerTransform GetTransform() { return transform_; }
		void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	private:
		Vector3 moveDirect_{};

		float acceleration_ = 0.0f;

	public: // 共通
		static float sAcceleration;

	};

}
