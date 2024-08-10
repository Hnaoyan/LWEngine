#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include <memory>
#include <variant>

class Boss;
class Player;

namespace BossState
{
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
		Boss* boss_ = nullptr;

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

	// 通常射撃状態
	class MissileAttackState : public IState 
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:
		// 移動に遷移するタイマー
		FrameTimer changeTimer_;
		FrameTimer fireTimer_;
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
		// 攻撃に遷移するタイマー
		FrameTimer changeTimer_;
	public:
		void TestProcess();

	};

	// 上下状態
	class UpDownState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:
		// 移動に遷移するタイマー
		FrameTimer changeTimer_;

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
		// 移動に遷移するタイマー
		FrameTimer changeTimer_;

	private:


	};

	using StateVariant = std::variant<MissileAttackState*, MoveState*, UpDownState*>;

	class StateDecider {
	private:
		Boss* boss_ = nullptr;
		Player* player_ = nullptr;
	public:

		void Initialize(Boss* boss, Player* player);
		void StateDecide(StateVariant nowState);

	private:
		void DefaultLoop(StateVariant nowState);

	};

}
