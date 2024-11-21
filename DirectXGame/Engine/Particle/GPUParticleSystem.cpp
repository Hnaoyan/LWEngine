#include "GPUParticleSystem.h"
#include <imgui.h>

GPUParticleManager::GPUParticleManager()
{

}

void GPUParticleManager::Initialize(Model* model)
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

	addEmitterName_ = "Emitter";

	emitters_.clear();

}

void GPUParticleManager::Update()
{
	if (isCreate_) {
		isCreate_ = false;
		CreateEmitter(addEmitterName_ + std::to_string(nameNum_));
	}
	if (isDelete_) {
		isDelete_ = false;
		DeleteEmitter(addEmitterName_ + std::to_string(nameNum_));
	}

	// エミッターの削除
	for (std::unordered_map<std::string, std::unique_ptr<GPUParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end();) {
		if (it->second->IsDead()) {
			it = emitters_.erase(it);
		}
		else {
			++it;
		}
	}
	// エミッターの更新
	for (std::unordered_map<std::string, std::unique_ptr<GPUParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end(); ++it) {
		it->second->Update();
	}

}

void GPUParticleManager::Draw(ICamera* camera)
{
	// パーティクルの描画
	for (std::unordered_map<std::string, std::unique_ptr<GPUParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end(); ++it) {
		it->second->Draw(camera);
	}
}

void GPUParticleManager::ImGuiDraw()
{
	ImGui::Begin("GPUParticle");
	ImGui::Text("\n");
	if (ImGui::Button("Create")) {
		isCreate_ = true;
	}
	if (ImGui::Button("Delete")) {
		isDelete_ = true;
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

void GPUParticleManager::CreateEmitter(std::string tag)
{
	// タグの要素検索
	auto it = emitters_.find(tag);
	// ある場合上書き
	if (it != emitters_.end()) {
		std::unique_ptr<GPUParticleEmitter> instance = std::make_unique<GPUParticleEmitter>();
		instance->Initialize(model_);
		instance->RefreshData(createData_);

		emitters_.insert_or_assign(tag, std::move(instance));
		return;
	}
	// なかった場合インスタンス生成・登録
	std::unique_ptr<GPUParticleEmitter> instance = std::make_unique<GPUParticleEmitter>();
	instance->Initialize(model_);
	instance->RefreshData(createData_);
	emitters_.emplace(tag, std::move(instance));
}

void GPUParticleManager::CreateEmitter(std::unique_ptr<GPUParticleEmitter> instance, std::string tag)
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

void GPUParticleManager::DeleteEmitter(std::string tag)
{
	// タグの要素検索
	auto it = emitters_.find(tag);
	// あれば削除
	if (it != emitters_.end()) {
		emitters_.erase(tag);
	}
}

void GPUParticleManager::DeleteEmitter(GPUParticleEmitter* emitter)
{
	// 検索
	for (auto it = emitters_.begin(); it != emitters_.end(); ++it) {
		// ポインタと配列で比較
		if ((*it).second.get() == emitter) {
			emitters_.erase((*it).first);
		}
	}
}

GPUParticleEmitter* GPUParticleManager::FindEmitter(std::string tag)
{
	// 検索
	auto it = emitters_.find(tag);
	// ある場合
	if (it != emitters_.end()) {
		return it->second.get();
	}
	return nullptr;
}

void GPUParticleManager::DataReset()
{
	this->emitters_.clear();
}
