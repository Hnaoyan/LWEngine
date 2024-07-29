#include "LockOn.h"
#include "../../GameObjectLists.h"
#include "Engine/Math/MathLib.h"
#include "Engine/LwLib/LwEngineLib.h"
#include <cassert>
#include <imgui.h>

void LockOn::Initialize(Player* player)
{
	assert(player);
	player_ = player;

	data.threshold = 0.75f;
	data.minDistanceZ = 5.0f;
	data.maxDistanceZ = 50.0f;

}

void LockOn::Update()
{
	if (ExistTarget()) {
		if (target_->IsDead()) {
			target_ = nullptr;
		}
	}
}

void LockOn::ToggleLockOn(ICamera* camera)
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

void LockOn::ImGuiDraw() {
	ImGui::DragFloat("Threshold", &data.threshold, 0.01f);
	ImGui::DragFloat("min", &data.minDistanceZ, 0.01f);
	ImGui::DragFloat("max", &data.maxDistanceZ, 0.01f);
}

void LockOn::TargetRelease()
{
	// 対象をnullに
	target_ = nullptr;
}

void LockOn::SearchTarget(ICamera* camera)
{
	std::list<std::pair<float, SampleEnemy*>> targets;

	for (std::vector<std::unique_ptr<SampleEnemy>>::iterator it = enemys_->begin();
		it != enemys_->end(); ++it) {

		Vector3 cameraToEnemy = (*it)->worldTransform_.GetWorldPosition() - player_->camera_->transform_.translate;
		Vector3 cameraToPlayer = player_->worldTransform_.GetWorldPosition() - player_->camera_->transform_.translate;

		Vector3 enemyViewVector = Matrix4x4::TransformVector3((*it)->worldTransform_.GetWorldPosition(), camera->viewMatrix_);

		float dot = Vector3::Dot(Vector3::Normalize(cameraToEnemy), Vector3::Normalize(cameraToPlayer));

		Vector2 screenPosition = LwLib::WorldToScreen((*it)->worldTransform_.GetWorldPosition(), camera);

		if (dot > data.threshold && enemyViewVector.z >= data.minDistanceZ && enemyViewVector.z <= data.maxDistanceZ) {
			targets.emplace_back(std::make_pair(Vector3::Length(cameraToEnemy), (*it).get()));
		}

	}

	target_ = nullptr;
	if (!targets.empty()) {
		targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		// 一番近いのをソート
		target_ = targets.front().second;
	}
	if (boss_) {
		Vector3 cameraToEnemy = boss_->worldTransform_.GetWorldPosition() - player_->camera_->transform_.translate;
		Vector3 cameraToPlayer = player_->worldTransform_.GetWorldPosition() - player_->camera_->transform_.translate;

		Vector3 enemyViewVector = Matrix4x4::TransformVector3(boss_->worldTransform_.GetWorldPosition(), camera->viewMatrix_);

		float dot = Vector3::Dot(Vector3::Normalize(cameraToEnemy), Vector3::Normalize(cameraToPlayer));

		Vector2 screenPosition = LwLib::WorldToScreen(boss_->worldTransform_.GetWorldPosition(), camera);

		if (dot > data.threshold && enemyViewVector.z >= data.minDistanceZ && enemyViewVector.z <= data.maxDistanceZ) {
			if (!targets.empty()) {
				if (Vector3::Length(cameraToEnemy) < targets.front().first) {
					target_ = boss_;
				}
			}
			else {
				target_ = boss_;
			}
		}
	}

}
