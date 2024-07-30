#pragma once
#include "Engine/Math/MathLib.h"
#include <memory>

class Boss;

namespace BossState
{
	/// <summary>
	/// 基のステートクラス
	/// </summary>
	class IState {
	public:
		virtual ~IState() = default;

	public:
		void PreInitialize(Boss* boss);
		virtual void Initialize() = 0;
		virtual void Update() = 0;
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


	class MissileAttackState : public IState 
	{
		void Initialize() override;
		void Update() override;
		void Exit() override;
	};


}
