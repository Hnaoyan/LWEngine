#include "GPUParticleSystem.h"
#include <imgui.h>

void GPUParticleSystem::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	//CreateEmitter();
	createData_.count = 24;
	createData_.frequency = 0.5f;
	createData_.frequencyTime = 0.0f;
	createData_.translate = {};
	createData_.radius = 1.0f;
	createData_.emit = 0;

}

void GPUParticleSystem::Update()
{
	// エミッターの削除
	for (std::unordered_map<std::string, std::unique_ptr<ParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end();) {
		if (it->second->IsDead()) {
			it = emitters_.erase(it);
		}
		else {
			++it;
		}
	}
	// エミッターの更新
	for (std::unordered_map<std::string, std::unique_ptr<ParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end(); ++it) {
		it->second->Update();
	}

}

void GPUParticleSystem::Draw(ICamera* camera)
{
	// パーティクルの描画
	for (std::unordered_map<std::string, std::unique_ptr<ParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end(); ++it) {
		it->second->Draw(camera);
	}
}

void GPUParticleSystem::ImGuiDraw()
{
	ImGui::Begin("GPUParticle");
	ImGui::Text("\n");
	addEmitterName_ = "Emitter";
	if (ImGui::Button("Create")) {
		CreateEmitter(addEmitterName_ + std::to_string(nameNum_));
	}
	if (ImGui::Button("Delete")) {
		DeleteEmitter(addEmitterName_ + std::to_string(nameNum_));
	}
	ImGui::Separator();
	ImGui::InputInt("AddNameNum", &nameNum_, 1);
	if (ImGui::TreeNode("CreateData")) {
		ImGui::DragFloat3("Position", &createData_.translate.x, 0.01f);
		ImGui::DragFloat("Radius", &createData_.radius, 0.01f);
		int instance = createData_.count;
		ImGui::DragFloat("Frequency", &createData_.frequency, 0.01f);
		ImGui::InputInt("MaxCount", &instance, 1);
		createData_.count = instance;

		ImGui::TreePop();
	}

	ImGui::End();
}

void GPUParticleSystem::CreateEmitter(std::string tag)
{
	// タグの要素検索
	auto it = emitters_.find(tag);
	// ある場合上書き
	if (it != emitters_.end()) {
		std::unique_ptr<ParticleEmitter> instance = std::make_unique<ParticleEmitter>();
		instance->Initialize(model_);
		instance->RefreshData(createData_);

		emitters_.insert_or_assign(tag, std::move(instance));
		return;
	}
	// なかった場合インスタンス生成・登録
	std::unique_ptr<ParticleEmitter> instance = std::make_unique<ParticleEmitter>();
	instance->Initialize(model_);
	instance->RefreshData(createData_);
	emitters_.emplace(tag, std::move(instance));
}

void GPUParticleSystem::CreateEmitter(std::unique_ptr<ParticleEmitter> instance, std::string tag)
{
	// タグの要素検索
	auto it = emitters_.find(tag);
	// ある場合上書き
	if (it != emitters_.end()) {
		emitters_.insert_or_assign(tag, std::move(instance));
		return;
	}
	// 追加
	emitters_.emplace(tag, std::move(instance));
}

void GPUParticleSystem::DeleteEmitter(std::string tag)
{
	// タグの要素検索
	auto it = emitters_.find(tag);
	// あれば削除
	if (it != emitters_.end()) {
		emitters_.erase(tag);
	}
}

ParticleEmitter* GPUParticleSystem::FindEmitter(std::string tag)
{
	// 検索
	auto it = emitters_.find(tag);
	// ある場合
	if (it != emitters_.end()) {
		return it->second.get();
	}
	return nullptr;
}
