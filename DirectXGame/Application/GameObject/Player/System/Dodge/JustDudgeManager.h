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
			// 維持する時間（コンボがつながる時間
			FrameTimer keepTimer;
			// 現在コンボ数
			int32_t nowCombo = 0;
			// 最大コンボ数
			int32_t maxCombo = 3;
			// 攻撃の種類変更
			bool isChangeAttack_ = false;
		};
		// プレイヤー
		Player* player_ = nullptr;

	public:
		JustDodgeManager() {};
		JustDodgeManager(Player* player);

		// ジャスト回避受付開始処理
		void DodgeExcept(const float& frame) {
			dodgeTimer_.Start(frame);
		}
		// ジャスト回避処理（内部フレーム
		void DodgeExcept();
		// ジャスト回避の開始
		void InvisibleExcept(const float& frame);
		// 更新
		void Update();
		// ImGui
		void ImGuiDraw();
		// 攻撃のダメージ量に変化を加えるかどうか
		bool IsChangeAttack() { return comboData_.isChangeAttack_; }
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
	public:
		void ComboReset() {
			comboData_.keepTimer.End();
			comboData_.nowCombo = 0;
		}
	
	private:
		// 回避を受け付けている時間
		FrameTimer dodgeTimer_;
		// 無敵（スローモーション）時間
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

