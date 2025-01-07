#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Application/GameObject/Boss/Animation/BossAnimationManager.h"
#include "Application/GameObject/Boss/System/BossSystemStructs.h"
#include "Application/GameObject/Bullet/BulletEnums.h"

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
class BulletBuilder;

namespace BossState {
	class AttackState;
	class MoveState;
	class OrbitMoveState;
	class UpDownState;
	class WaitState;
	class TeleportState;
	class MissileAttackState;
	class TurnMissileState;
	class SystemDownState;
	class MissileBarrageState;
	class MissileWaveState;
	class MissileContainerState;
}

namespace BossSystemContext {
	class BulletCluster;
}

/// <summary>
/// ボスのステート関係
/// </summary>
namespace BossState
{
	// クラスのリスト
	using StateVariant = std::variant<AttackState*, MoveState*, UpDownState*,
		WaitState*, TeleportState*, MissileAttackState*, OrbitMoveState*,
		SystemDownState*, MissileBarrageState*, MissileWaveState*, TurnMissileState*,
		MissileContainerState*>;

	/// <summary>
	/// 発射関係の処理をまとめたコンポーネント
	/// </summary>
	class BulletComponent
	{
	public:
		void Initialize(Boss* boss);
		void GenerateTracking(const Vector3& direct, TrackingAttribute type);
		void Fire(BulletBuilder builder, TrackingAttribute type);

	private:
		Boss* boss_ = nullptr;

	};

	/// <summary>
	/// 基のステートクラス
	/// </summary>
	class IState {
	public:
		virtual ~IState() = default;

		using AnimType = BossSystemContext::AnimationManager::AnimType;

	public:
		/// <summary>
		/// 共通の初期化
		/// </summary>
		/// <param name="boss"></param>
		void PreInitialize(Boss* boss);

		// 先行の予備動作
		virtual void PreAction() {};
		// 初期化
		virtual void Initialize();
		// 更新
		virtual void Update() = 0;
		// 終了処理
		virtual void Exit() = 0;
	public:
		void SetChangeTimer(const float& time) { changeTimer_.Start(time); }

	protected:
		// ボス本体の向きの処理
		void RotateUpdate();
		// タイマーの処理
		void TimerUpdate(StateVariant state);
		// 通常の射撃を生成する関数
		void GenerateBullet(const Vector3& direct, const float& speed);

		Boss* boss_ = nullptr;
		// 変更のフレームタイマー
		FrameTimer changeTimer_;
		// 予備動作の時間
		FrameTimer preActionTimer_;
		// 追従のコンポーネント
		BulletComponent bulletComponent_;

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

#pragma region State達

	// バリア割れの硬直
	class SystemDownState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;

	};

	// ミサイルの一斉射撃
	class MissileAttackState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:
		void MissileAttack();
		void GenerateMissile(const Matrix4x4& rotateMatrix, TrackingAttribute type);
		void GenerateMissile(const Vector3& direct, TrackingAttribute type);
	private:
		Vector3 bulletDirect_ = {};

		BossSystemContext::BulletCluster* cluster_ = nullptr;

		// 
		BossSystemContext::BulletCluster* firstPhaseCluster_ = nullptr;
		BossSystemContext::BulletCluster* secondPhaseCluster_ = nullptr;

		FrameTimer attackTimer_;
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
			kPredictive,	// 予測
			kStraight,	// 直進
			kSpread,	// 広域
			kRadialFire,	// 円
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
		float fireCooltime_ = 0.0f;	// クールタイム
		FrameTimer fireTimer_;	// 射撃時間

		Vector3 bulletDirect_ = {};	// 向き
		float bulletSpeed_ = 0.0f;	// 速度
		float bulletScale_ = 0.5f;	// スケール

		// 種類
		ShotPattern pattern_ = ShotPattern::kPredictive;

		float startRotate_ = 0.0f;	// 開始角度
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

	// 円形移動
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
		FourPoint controlPoint = {};
		// 回転のパターン
		QuaterRotationPattern quaterPattern_ = QuaterRotationPattern::kFirst;

		bool isEnd_ = false;
		FrameTimer curveTimer_;
		Vector3 startPosition_ = {};
		Vector3 controlPosition_ = {};
		Vector3 endPosition_ = {};

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
