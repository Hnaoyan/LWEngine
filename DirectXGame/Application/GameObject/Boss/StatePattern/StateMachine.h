#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include <memory>
#include <variant>
#include <map>
#include <vector>
#include <string>

/// <summary>
/// 前方宣言
/// </summary>
class Boss;
class Player;
namespace BossState {
	class AttackState;
	class MoveState;
	class UpDownState;
	class WaitState;
	class TeleportState;
	class MissileAttackState;
}

/// <summary>
/// ボスのステート関係
/// </summary>
namespace BossState
{
	// クラスのリスト
	using StateVariant = std::variant<AttackState*, MoveState*, UpDownState*, WaitState*, TeleportState*, MissileAttackState*>;

	/// <summary>
	/// 基のステートクラス
	/// </summary>
	class IState {
	public:
		virtual ~IState() = default;
	public:
		/// <summary>
		/// 共通の初期化
		/// </summary>
		/// <param name="boss"></param>
		void PreInitialize(Boss* boss);
		// 初期化
		virtual void Initialize() = 0;
		// 更新
		virtual void Update() = 0;
		// 終了処理
		virtual void Exit() = 0;
	protected:
		// タイマーの処理
		void TimerUpdate(StateVariant state);

		Boss* boss_ = nullptr;
		// 変更のフレームタイマー
		FrameTimer changeTimer_;
	};

	/// <summary>
	/// 管理クラス
	/// </summary>
	class StateManager {
	private:
		using IState = BossState::IState;

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="boss"></param>
		void Initialize(Boss* boss);
		/// <summary>
		/// 変更処理
		/// </summary>
		/// <param name="newState"></param>
		void ChangeRequest(std::unique_ptr<IState> newState);
	private:
		Boss* boss_ = nullptr;
	};
	/// <summary>
	/// ステート変更管理クラス
	/// </summary>
	class StateDecider {
	public:
		enum class StatePattern : uint32_t{
			kAttack,
			kMove,
			kUpdown,
			kWait,
			kTeleport,
			kMissile,
			kMax,
		};
		struct StateObject {
			std::vector<StateDecider::StatePattern> patterns;
			uint32_t number;
			uint32_t maxStep;
		};
	private:
		Boss* boss_ = nullptr;
		Player* player_ = nullptr;

		std::string tableTag_;

		void RandomTable(uint32_t random) {
			if (random == 0) {
				tableTag_ = "Default";
			}
			else if (random == 1) {
				tableTag_ = "MoveType";
			}
			else {
				tableTag_ = "AttackType";
			}
		}

	public:

		void Initialize(Boss* boss, Player* player);
		void StateDecide(StateVariant nowState);

	private:
		// ステートの変更処理
		void StateSelect(StatePattern number);

	private:
		// テーブル中か
		bool IsInActionSequence_;
		bool isCooltime_ = false;
		uint32_t randomValue_ = 0;
		// テーブル内の位置
		uint32_t currentStep_;
		std::map<std::string, StateObject> tables_;
	};

	class MissileAttackState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:
		void MissileAttack();
	private:
		Vector3 bulletDirect_ = {};
		float bulletSpeed_ = 0.0f;
		float bulletScale_ = 0.5f;
	};

	// 通常射撃状態
	class AttackState : public IState 
	{
	public:
		/// <summary>
		/// 射撃のパターン
		/// </summary>
		enum class ShotPattern : uint32_t
		{
			kPredictive,
			kStraight,
			kSpread,
		};

	public: // 継承
		void Initialize() override;
		void Update() override;
		void Exit() override;
	public:
		// 生成時の設定関数
		void SimpleAttack(const Vector3& position);

	private: 
		// プレイヤーに向けた射撃
		void LockAttack();
		// 生成場所に応じた直線射撃
		void StraightAttack();
		// 拡散
		void SpreadAttack();

		// 弾の生成部分の大枠
		void GenerateProcess();
	private:
		FrameTimer fireTimer_;

		Vector3 bulletDirect_ = {};
		float bulletSpeed_ = 0.0f;
		float bulletScale_ = 0.5f;

		float rotateAngle_ = 0.0f;

		ShotPattern pattern_;

	};

	// 移動状態
	class MoveState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:
		bool isLeft_ = false;

		// X,Z平面の最大の値
		const float kMaxPoint = 200.0f;

		Vector3 startPosition_{};
		Vector3 endPosition_{};

	public:
		void MoveSelect(const Vector3& playerPosition);
		void TestProcess();

	};

	// 瞬間移動状態
	class TeleportState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;

	public:
		void SelectTeleportPoint(const Vector3& position);

	private:
		// X,Z平面の最大の値
		const float kMaxPoint = 60.0f;

		Vector3 startPosition_{};
		Vector3 endPosition_{};

		bool isCross_ = false;

	};

	// 上下状態
	class UpDownState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:
		Vector3 startPosition_{};
		Vector3 endPosition_{};

	};

	// 待機状態
	class WaitState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:

	public:
		void SetTimer(float endFrame) {
			changeTimer_.Start(endFrame);
		}

	};

}
