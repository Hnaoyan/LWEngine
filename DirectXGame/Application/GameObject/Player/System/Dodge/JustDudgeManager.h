#pragma once
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Engine/3D/Drawer/Model.h"
#include <memory>
#include "../IPlayerSystem.h"

class Player;

namespace PlayerContext
{
	/// <summary>
	/// ジャスト回避管理
	/// </summary>
	class JustDodgeManager : public ISystem
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

		// 回避用の距離を分かるようにするコライダーモデルの情報
		struct DodgeCollider
		{
			WorldTransform transform;
			uint32_t texture = 0u;
			std::unique_ptr<Material> material;
			void Update() {
				transform.UpdateMatrix();
				material->Update();
			}
		};

		// 回避・無敵関係の情報
		struct InvisibleData
		{
			// 無敵（スローモーション）時間
			FrameTimer activeTimer;
			// 無敵後の余裕を持たせるタイマー
			FrameTimer afterTimer;
			// 無敵から基にもどる
			FrameTimer returnTimer;
			void Update() {
				activeTimer.Update();
				afterTimer.Update();
				returnTimer.Update();
			}

			bool receptionFlag = false;
			float delayFrame = 0.0f;
		};

		// プレイヤー
		Player* player_ = nullptr;

	public:
		JustDodgeManager() {};
		JustDodgeManager(Player* player);
		// 初期化
		void Initialize(Player* player) override;
		// 更新
		void Update() override;
		// 描画
		void Draw(ModelDrawDesc desc);
		// ImGui
		void ImGuiDraw();
	public:
		// ジャスト回避受付開始処理
		void DodgeExcept(const float& frame) { dodgeTimer_.Start(frame); }
		// ジャスト回避処理（内部フレーム
		void DodgeExcept();
		// ジャスト回避の開始
		void InvisibleExcept(const float& frame);
		// 回避カウント
		void DodgeCountIncrement() { dodgeCount_++; }
	public:	// アクセッサ
		// 攻撃のダメージ量に変化を加えるかどうか
		bool IsChangeAttack() { return comboData_.isChangeAttack_; }
		// ジャスト回避可能状態
		bool IsActive() { return dodgeTimer_.IsActive(); }
		// 無敵状態
		bool IsInvisibleActive() { return invisible_.activeTimer.IsActive(); }
		// ジャスト回避の後の無敵猶予
		bool IsInvisibleAfter() { return invisible_.afterTimer.IsActive(); }
		// 無敵の終了タイミング
		bool EndInvisible() { return invisible_.activeTimer.IsEnd(); }
		// コライダー
		Sphere* GetCollider() { return &collider_; }
		// コライダーの設定
		void SetPosition(const Vector3& position) { colliderPosition_ = position; }
		// コンボの維持するタイマー
		FrameTimer* GetComboKeepTimer() { return &comboData_.keepTimer; }
	public:
		// コンボデータの初期化
		void ComboReset() {
			comboData_.keepTimer.End();
			comboData_.nowCombo = 0;
		}
	
	private:
		// 回避を受け付けている時間
		FrameTimer dodgeTimer_;
		// 無敵の情報
		InvisibleData invisible_;
		// ジャスト回避フレーム
		float justDodgeFrame_ = 10.0f;
		// スローモーションの値
		float slowFactor_ = 30.0f;
		// ダッシュ時の残像判定
		Vector3 colliderPosition_{};
		// スフィアコライダー
		Sphere collider_;
		// 回避コンボの情報
		ComboData comboData_{};
		// 回避範囲表示のオブジェクト
		DodgeCollider dodgeColliderObject;
		// 回避した際に追従を振り切った数
		int32_t dodgeCount_ = 0;

	};
}

