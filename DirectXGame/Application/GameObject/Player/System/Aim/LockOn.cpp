#include "LockOn.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/Math/MathLib.h"
#include "Engine/LwLib/LwLibrary.h"
#include <cassert>
#include <imgui.h>

void PlayerContext::LockOn::Initialize(Player* player)
{
	assert(player);
	player_ = player;

	data.threshold = 0.75f;
	data.minDistanceZ = 5.0f;
	data.maxDistanceZ = 2000.0f;

	// 対象の初期化
	target_ = nullptr;
}

void PlayerContext::LockOn::Update()
{
	if (ExistTarget()) {
		if (target_->IsDead()) {
			target_ = nullptr;
		}
	}

	transitionTimer_.Update();
}

void PlayerContext::LockOn::ToggleLockOn(ICamera* camera)
{
	// ターゲット処理
	if (target_ == nullptr) {
		SearchTarget(camera);
	}
	// 解除処理
	else {
		TargetRelease();
	}
}

void PlayerContext::LockOn::ChangeLockOnTarget(ICamera* camera)
{

	ChangeTarget(camera);

}

void PlayerContext::LockOn::ImGuiDraw() {
	ImGui::DragFloat("Threshold", &data.threshold, 0.01f);
	ImGui::DragFloat("min", &data.minDistanceZ, 0.01f);
	ImGui::DragFloat("max", &data.maxDistanceZ, 0.01f);
}

void PlayerContext::LockOn::TargetRelease()
{
	// 対象をnullに
	target_ = nullptr;
}

void PlayerContext::LockOn::SearchTarget(ICamera* camera)
{
	// ボスがいれば
	if (boss_) {
		Vector3 cameraToEnemy = boss_->worldTransform_.GetWorldPosition() - player_->GetCamera()->transform_.translate;
		Vector3 cameraToPlayer = player_->worldTransform_.GetWorldPosition() - player_->GetCamera()->transform_.translate;

		Vector3 enemyViewVector = Matrix4x4::TransformVector3(boss_->worldTransform_.GetWorldPosition(), camera->viewMatrix_);

		float dot = Vector3::Dot(Vector3::Normalize(cameraToEnemy), Vector3::Normalize(cameraToPlayer));

		Vector2 screenPosition = LwLib::WorldToScreen(boss_->worldTransform_.GetWorldPosition(), camera);

		// 条件を満たせばリストのやつと比較
		if (dot > data.threshold && enemyViewVector.z >= data.minDistanceZ && enemyViewVector.z <= data.maxDistanceZ * 2.5f) {
			//if (!targets.empty()) {
			//	if (Vector3::Length(cameraToEnemy) < targets.front().first) {
			//		target_ = boss_;
			//	}
			//}
			//else {
			//	target_ = boss_;
			//}

			target_ = boss_;
			transitionTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "LockDuration") / 2.0f);
		}
	}

}

void PlayerContext::LockOn::ChangeTarget(ICamera* camera)
{
	//std::list<std::pair<float, SampleEnemy*>> targets;
	//if (enemys_) {
	//	for (std::vector<std::unique_ptr<SampleEnemy>>::iterator it = enemys_->begin();
	//		it != enemys_->end(); ++it) {

	//		Vector3 cameraToEnemy = (*it)->worldTransform_.GetWorldPosition() - player_->GetCamera()->transform_.translate;
	//		Vector3 cameraToPlayer = player_->worldTransform_.GetWorldPosition() - player_->GetCamera()->transform_.translate;

	//		Vector3 enemyViewVector = Matrix4x4::TransformVector3((*it)->worldTransform_.GetWorldPosition(), camera->viewMatrix_);

	//		float dot = Vector3::Dot(Vector3::Normalize(cameraToEnemy), Vector3::Normalize(cameraToPlayer));

	//		Vector2 screenPosition = LwLib::WorldToScreen((*it)->worldTransform_.GetWorldPosition(), camera);

	//		if (dot > data.threshold && enemyViewVector.z >= data.minDistanceZ && enemyViewVector.z <= data.maxDistanceZ) {
	//			targets.emplace_back(std::make_pair(Vector3::Length(cameraToEnemy), (*it).get()));
	//		}

	//	}

	//	if (!targets.empty()) {
	//		targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
	//		// 一番近いのをソート
	//		if (target_ == targets.front().second && targets.size() > 1) {
	//			auto it = targets.begin();
	//			++it;
	//			target_ = (*it).second;
	//		}
	//		else {
	//			target_ = targets.front().second;
	//		}
	//	}
	//}

	if (boss_) {
		Vector3 cameraToEnemy = boss_->worldTransform_.GetWorldPosition() - player_->GetCamera()->transform_.translate;
		Vector3 cameraToPlayer = player_->worldTransform_.GetWorldPosition() - player_->GetCamera()->transform_.translate;

		Vector3 enemyViewVector = Matrix4x4::TransformVector3(boss_->worldTransform_.GetWorldPosition(), camera->viewMatrix_);

		float dot = Vector3::Dot(Vector3::Normalize(cameraToEnemy), Vector3::Normalize(cameraToPlayer));

		Vector2 screenPosition = LwLib::WorldToScreen(boss_->worldTransform_.GetWorldPosition(), camera);

		if (dot > data.threshold && enemyViewVector.z >= data.minDistanceZ && enemyViewVector.z <= data.maxDistanceZ * 2.5f) {
			//if (!targets.empty()) {
			//	if (Vector3::Length(cameraToEnemy) < targets.front().first) {
			//		target_ = boss_;
			//	}
			//}
			//else {
				target_ = boss_;
			//}
		}
	}
}
