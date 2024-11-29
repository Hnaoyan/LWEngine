#pragma once
#include <stdint.h>
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/Math/MathLib.h"

class Player;

namespace PlayerContext
{
	/// <summary>
	/// 体力のマネージャ
	/// </summary>
	class HealthManager
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="player"></param>
		/// <param name="maxHP"></param>
		void Initialize(Player* player, uint32_t maxHP);
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		/// <summary>
		/// ダメージ処理
		/// </summary>
		/// <param name="damage"></param>
		void TakeDamage(uint32_t damage = 1);
		/// <summary>
		/// ImGui
		/// </summary>
		void ImGuiDraw();

	public: // アクセッサ
		bool IsInvisible() { return invisibleEffect_.activeTimer.IsActive(); }
		bool EndInvisible() { return invisibleEffect_.activeTimer.IsEnd(); }
		float GetHPRatio() { return (float)hitPoint_.currentHealth / (float)hitPoint_.maxHealth; }
		float GetAlpha() { return invisibleEffect_.color.w; }
	private:
		// HPのデータ
		struct HealthData {
			uint32_t maxHealth = 0;
			uint32_t currentHealth = 0;
			FrameTimer damageEffectDuration;
			float invisibleFrame = 60.0f;
		};

		// 無敵状態のエフェクトクラス
		struct InvisibleEffect {
			void Update() {
				activeTimer.Update();
				upTimer.Update();
				downTimer.Update();
			}
			FrameTimer activeTimer; // 無敵時間
			FrameTimer upTimer;	// 0→1になる
			FrameTimer downTimer;	// 1→0になる
			Vector4 color;	// 状態のカラー
			float returnFrame = 8.0f;	// 返るフレーム
		};
		// HP関係
		HealthData hitPoint_;
		// 無敵表現
		InvisibleEffect invisibleEffect_;

		Player* player_ = nullptr;

	};

}
