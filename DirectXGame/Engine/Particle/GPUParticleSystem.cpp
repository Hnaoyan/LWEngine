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
	for (std::vector<std::unique_ptr<ParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end(); ++it) {
		(*it)->Update();
	}
}

void GPUParticleSystem::Draw(ICamera* camera)
{
	for (std::vector<std::unique_ptr<ParticleEmitter>>::iterator it = emitters_.begin(); it != emitters_.end(); ++it) {
		(*it)->Draw(camera);
	}
}

void GPUParticleSystem::ImGuiDraw()
{
	ImGui::Begin("GPUParticle");
	ImGui::Text("\n");
	if (ImGui::Button("Create")) {
		CreateEmitter();
	}
	if (ImGui::Button("Delete")) {
		DeleteEmitter();
	}
	ImGui::Separator();
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

void GPUParticleSystem::CreateEmitter()
{
	std::unique_ptr<ParticleEmitter> instance = std::make_unique<ParticleEmitter>();
	instance->Initialize(model_);
	instance->UpdateEmitter(createData_);
	emitters_.push_back(std::move(instance));
}

void GPUParticleSystem::DeleteEmitter()
{
	if (!emitters_.empty()) {
		emitters_.erase(emitters_.begin());
	}
}
