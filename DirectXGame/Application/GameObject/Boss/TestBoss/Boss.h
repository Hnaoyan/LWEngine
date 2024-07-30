#pragma once
#include "../../IGameObject.h"
#include "../StatePattern/StateMachine.h"
#include "../System/BossSystem.h"

class Boss : public IGameObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc) override;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	void ImGuiDraw() override;
	/// <summary>
	/// コールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderObject target) override;
public:
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Sphere* GetCollider() { return &collider_; }


private:
	Sphere collider_;
	BossState::StateManager stateManager_;
	BossSystemContext::HealthManager healthManager_;

	std::unique_ptr<BossState::IState> state_;
public:
	BossState::IState* GetState() { return state_.get(); }
	void ChangeState(std::unique_ptr<BossState::IState> newState) {
		state_ = std::move(newState);
	}
};
