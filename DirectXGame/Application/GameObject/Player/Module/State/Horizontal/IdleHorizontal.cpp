#include "IdleHorizontal.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameSystem/GameSystem.h"

void IdleHorizontal::Initialize()
{
	nowState_ = this;
}

void IdleHorizontal::Update()
{
	//player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	//player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);

	//Vector3 arrowKey{};
	//if (input_->PressKey(DIK_UP)) {
	//	arrowKey.z += 1.0f;
	//}
	//else if (input_->PressKey(DIK_DOWN)) {
	//	arrowKey.z -= 1.0f;
	//}
	//if (input_->PressKey(DIK_LEFT)) {
	//	arrowKey.x -= 1.0f;
	//}
	//else if (input_->PressKey(DIK_RIGHT)) {
	//	arrowKey.x += 1.0f;
	//}

	//float speed = 10.0f;
	//arrowKey = Vector3::Normalize(arrowKey);
	//if (arrowKey.x != 0.0f) {
	//	player_->velocity_.x += arrowKey.x * GameSystem::GameSpeedFactor() * speed;
	//}
	//if (arrowKey.z != 0.0f) {
	//	player_->velocity_.z += arrowKey.z * GameSystem::GameSpeedFactor() * speed;
	//}

	if (isLeftStickActive_) {
		stateManager_->ChangeRequest(StateManager::kMove);
		return;
	}
}

void IdleHorizontal::Exit()
{

}

void IdleHorizontal::InputHandle()
{
	// 入力
	IPlayerState::InputHandle();

}
