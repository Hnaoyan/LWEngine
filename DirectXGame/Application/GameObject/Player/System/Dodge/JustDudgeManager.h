#pragma once
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class Player;

namespace PlayerContext
{

	class JustDodgeManager
	{
	private:
		// コンボの情報
		struct ComboData
		{
			// コンボがつながるフレームカウント
			FrameTimer keepTimer;
			int32_t count = 0;
		};
		// プレイヤー
		Player* player_ = nullptr;

	public:
		JustDodgeManager() {};
		JustDodgeManager(Player* player);

		// ジャスト回避開始処理
		void DodgeExcept(const float& frame) {
			dodgeTimer_.Start(frame);
		}
		// ジャスト回避処理（内部フレーム
		void DodgeExcept();
		void InvisibleExcept(const float& frame) {
			invisibleTimer_.Start(frame);
		}
		// 更新
		void Update();
		// ImGui
		void ImGuiDraw();

		// ジャスト回避可能状態
		bool IsActive() { return dodgeTimer_.IsActive(); }
		// 無敵状態
		bool IsInvisible() { return invisibleTimer_.IsActive(); }
		// 無敵の終了タイミング
		bool EndInvisible() { return invisibleTimer_.IsEnd(); }
		// コライダー
		Sphere* GetCollider() { return &collider_; }
		// コライダーの設定
		void SetPosition(const Vector3& position) { colliderPosition_ = position; }
	private:
		// ドッジ出来る時間
		FrameTimer dodgeTimer_;
		// 無敵時間
		FrameTimer invisibleTimer_;
		// 無敵から基にもどる
		FrameTimer invisibleReturnTimer_;
		// ジャスト回避フレーム
		float justDodgeFrame_ = 10.0f;
		// ダッシュ時の残像判定
		Vector3 colliderPosition_{};
		// スフィアコライダー
		Sphere collider_;
		// スローモーションの値
		float slowFactor_ = 30.0f;
		// 回避コンボの情報
		ComboData comboData_{};
	};
}

