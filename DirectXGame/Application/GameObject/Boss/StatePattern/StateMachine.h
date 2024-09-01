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
	class OrbitMoveState;
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
	using StateVariant = std::variant<AttackState*, MoveState*, UpDownState*, WaitState*, TeleportState*, MissileAttackState*, OrbitMoveState*>;

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

		// 先行の予備動作
		virtual void PreAction() {};

		// 初期化
		virtual void Initialize() = 0;
		// 更新
		virtual void Update() = 0;
		// 終了処理
		virtual void Exit() = 0;
	protected:
		// ボス本体の向きの処理
		void RotateUpdate();

		// タイマーの処理
		void TimerUpdate(StateVariant state);

		Boss* boss_ = nullptr;
		// 変更のフレームタイマー
		FrameTimer changeTimer_;
		// 予備動作の時間
		FrameTimer preActionTimer_;

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
			kOrbitMove,
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
		// テーブルの決定
		void TableSelect(std::string tableTag);

	private:
		// テーブル中か
		bool IsInActionSequence_ = false;
		bool isCooltime_ = false;
		uint32_t randomValue_ = 0;
		// テーブル内の位置
		uint32_t currentStep_ = 0;
		std::map<std::string, StateObject> tables_;

		std::string currentTabletag_;

		std::vector<std::string> section_;
		uint32_t sectionIndex_ = 0;
	};

#pragma region State達
	// ミサイルの攻撃
	class MissileAttackState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:
		void MissileAttack();

		void DefaultMissile();

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
			kRadialFire,
		};

	public: // 継承
		void Initialize() override;
		void Update() override;
		void Exit() override;
	public:
		// 生成時の設定関数
		void SimpleAttack(const Vector3& position);

		void Setting(float stateTimer, ShotPattern pattern);

	private:
		// プレイヤーに向けた射撃
		void LockAttack();
		// 生成場所に応じた直線射撃
		void StraightAttack();
		// 拡散
		void SpreadAttack();
		// 全方位弾
		void RadialFireAttack();
		// 弾の生成部分の大枠
		void GenerateProcess();
	private:
		float fireCooltime_ = 0.0f;
		FrameTimer fireTimer_;

		Vector3 bulletDirect_ = {};
		float bulletSpeed_ = 0.0f;
		float bulletScale_ = 0.5f;

		ShotPattern pattern_;

		float startRotate_ = 0.0f;

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

	class OrbitMoveState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;

	public:
		enum class QuaterRotationPattern : uint32_t {
			kFirst,
			kSecond,
			kThird,
			kFourth,
		};
		/// <summary>
		/// 生成時に呼ぶ関数
		/// </summary>
		void GenerateMovePoint(float length, QuaterRotationPattern pattern);

	private:
		struct FourPoint
		{
			Vector2 LFront, LBack, RFront, RBack;
		};
		// 制御点
		FourPoint controlPoint;
		// 回転のパターン
		QuaterRotationPattern quaterPattern_;

		bool isEnd_ = false;
		FrameTimer curveTimer_;
		Vector3 startPosition_{};
		Vector3 controlPosition_{};
		Vector3 endPosition_{};

	private: // 攻撃
		// 攻撃処理
		void LockOnAttack();

		FrameTimer fireTimer_;

		Vector3 bulletDirect_ = {};
		float bulletSpeed_ = 0.0f;
		float bulletScale_ = 0.5f;

		bool isAttack_ = false;

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

#pragma endregion

}
