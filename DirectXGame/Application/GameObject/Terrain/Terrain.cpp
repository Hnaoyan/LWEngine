#include "Terrain.h"
#include "imgui.h"

#include <string>

uint32_t Terrain::sSerialNumber = 0;

void Terrain::Initialize()
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	InstancedUnit::Initialize();
}

void Terrain::Update()
{
	InstancedUnit::Update();
}

void Terrain::ImGuiDraw()
{
	std::string name = "Position" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &transform_.translate.x, 0.01f);
	name = "Rotate" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &transform_.rotate.x, 0.01f);
	name = "Scale" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &transform_.scale.x, 0.01f);
}

void Terrain::OnCollision(ColliderObject object)
{
	object;
}
