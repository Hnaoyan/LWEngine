#pragma once
#include "Engine/3D/WorldTransform.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "../IPlayerSystem.h"

class Player;

namespace PlayerContext
{
	/// <summary>
	/// プレイヤーの簡易アニメーション関係
	/// </summary>
	class AnimationManager : public ISystem
	{
	private:
		struct FloatingData
		{
			// 浮遊ギミックの媒介変数
			float parameter = 0.0f;
			/// 浮遊の振幅<m>
			float width = 0.25f;
			/// 浮遊移動のサイクル<frame>
			int period = 60;

			// 戻る時間
			FrameTimer returnTimer;
			// アクティブフラグ
			bool isActive_ = false;
			// 開始値
			float start_ = 0.0f;
			// 初期化
			void Initialize() {
				parameter = 0.0f;
				width = 0.25f;
				period = 60;
			}
			// 更新
			void Update() {
				// 1フレームでのパラメータ加算値
				const float step = 2.0f * float(3.14f) / period;
				// パラメータを1ステップ分加算
				parameter += step;
				// 2πを超えたら0に戻す
				parameter = std::fmod(parameter, 2.0f * float(3.14f));
			}

		};

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="player"></param>
		void Initialize(Player* player) override;
		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;
		/// <summary>
		/// リセット
		/// </summary>
		void Reset();

		WorldTransform bodyTransform_;

	private:
		// 移動時のアニメーションの値
		FloatingData animParms_;
	};
}