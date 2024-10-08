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
	public: // アクセッサ
		void SetPlayer(Player* player) { player_ = player; }
		void SetTrackType(TrackType type) { trackType_ = type; }
		void SetBossPtr(Boss* boss) { boss_ = boss; }

	private:
		// 追跡している時間
		FrameTimer trackTimer_;
		// プレイヤーのポインタ
		Player* player_ = nullptr;
		// ボスのポインタ
		Boss* boss_ = nullptr;
		// 追尾の種類
		TrackType trackType_;
		// 補間の半径
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
	private:
		// 移動方向
		Vector3 moveDirect_{};

	public: // 共通
		static float sAcceleration;

	};

}
