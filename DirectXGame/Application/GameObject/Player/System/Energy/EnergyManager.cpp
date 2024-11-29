#include "EnergyManager.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/Player/Player.h"
#include <imgui.h>

void PlayerContext::EnergyManager::Initialize(Player* player)
{
	player_ = player;
	
	energy_.maxEnergy = 100.0f;
	energy_.currentEnergy = energy_.maxEnergy;

}

void PlayerContext::EnergyManager::Update()
{
	// タイマーの更新
	quickBoostRecoveryCoolTime_.Update();
	overheatTimer_.Update();
	//energy_.isAssending = std::holds_alternative<AssendingState*>(player_->VerticalState()->GetVariant());
	//if (energy_.isAssending) {
	//	float decrementAssending = 75.0f;
	//	energy_.currentEnergy -= decrementAssending * GameSystem::GameSpeedFactor();
	//}

	if (overheatTimer_.IsActive()) {
		energy_.currentEnergy = Ease::Easing(0.0f, energy_.maxEnergy, overheatTimer_.GetElapsedFrame());
	}
	else if(!quickBoostRecoveryCoolTime_.IsActive()) {
		if (energy_.maxEnergy > energy_.currentEnergy) {
			energy_.currentEnergy += energy_.recoveryValue * GameSystem::GameSpeedFactor();
		}
	}
}

void PlayerContext::EnergyManager::BoostDecrement()
{
	float defaultDecrement = (energy_.maxEnergy / decrementRate_);
	DecrementProcess(defaultDecrement);
}

void PlayerContext::EnergyManager::QuickBoostDecre()
{
	float defaultDecrement = (energy_.maxEnergy / decrementRate_);
	quickRate_ = 0.75f;
	DecrementProcess(defaultDecrement * quickRate_);
}

void PlayerContext::EnergyManager::ImGuiDraw()
{
	static float bar = 0.01f;
	ImGui::DragFloat("Bar", &bar, 0.01f);
	// 最大値
	ImGui::DragFloat("MaxEnergy", &energy_.maxEnergy, bar);
	// ダッシュの消費割合
	ImGui::DragFloat("DashEnergy", &decrementRate_, bar);
	// 回復量
	ImGui::DragFloat("RecoveryValue", &energy_.recoveryValue, bar);
	// ダッシュ後の回復までの時間
	ImGui::DragFloat("RecoveryFrame", &energy_.recoveryFrame, bar);
	if (ImGui::Button("Overheat")) {
		overheatTimer_.Start(60.0f * 3.0f);
		energy_.currentEnergy = 0.0f;
	}
}

void PlayerContext::EnergyManager::DecrementProcess(float value)
{
	energy_.currentEnergy -= (value);
	// 0を切った場合オーバーヒート
	if (energy_.currentEnergy <= 0.0f) {
		energy_.currentEnergy = 0.0f;
		// ３秒オーバーヒート
		overheatTimer_.Start(60.0f * 3.0f);
	}
	// 回復するまでのクールタイム
	quickBoostRecoveryCoolTime_.Start(energy_.recoveryFrame);
}
