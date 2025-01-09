#include "TurnMissileState.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void BossState::TurnMissileState::Initialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	boss_->SetNowVariantState(this);
	// 開くアニメーションの受付
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, global->GetValue<float>("BossAnimation", "OpenFrame"));

	// アクション前の待機タイマー
	float preFrame = 60.0f;
	preActionTimer_.Start(preFrame);

	// 基底初期化
	IState::Initialize();
}

void BossState::TurnMissileState::Update()
{

	TimerUpdate(this);
}

void BossState::TurnMissileState::Exit()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	boss_->SetPrevVariantState(this);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, global->GetValue<float>("BossAnimation", "CloseFrame"));
}
