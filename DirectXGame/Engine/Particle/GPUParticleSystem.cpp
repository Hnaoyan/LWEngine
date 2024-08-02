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
	for (std::unordered_map<std::string, std::unique_ptr<ParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end(); ++it) {
		it->second->Update();
	}
	
	DirectXCommand::ExecuteCommandList(DirectXCommand::sCommandList_.Get());
	//DirectXCommand::ResetCloseCommandList(DirectXCommand::sCommandList_.Get());

}

void GPUParticleSystem::Draw(ICamera* camera)
{
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
		instance->UpdateEmitter(createData_);

		emitters_.insert_or_assign(tag, std::move(instance));
		return;
	}

	std::unique_ptr<ParticleEmitter> instance = std::make_unique<ParticleEmitter>();
	instance->Initialize(model_);
	instance->UpdateEmitter(createData_);
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
