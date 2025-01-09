#include "LightingManager.h"
#include <imgui.h>

LightingManager::LightingManager()
{
	// ライト作成
	drLight_.reset(DirectionalLight::CreateLight());
	ptLight_.reset(PointLight::CreateLight());
	spLight_.reset(SpotLight::CreateLight());
}

void LightingManager::Initialize()
{
	// 平行光源データ
	drData_.color = { 1.0f,1.0f,1.0f,1.0f };
	drData_.direction = { 0.0f,1.0f,0.0f };
	drData_.intensity = 1.2f;

	// 点光源データ
	ptData_.intensity = 0.5f;
	ptData_.position = { 0,50.0f,0 };
	ptData_.color = { 1,1,1,1 };
	ptData_.decay = 10.0f;
	ptData_.radius = 300.0f;

	// 照光源データ
	spData_.color = { 1,1,1,1 };
	spData_.position = { 2.0f,200.0f,0.0f };
	spData_.distance = 300.0f;
	spData_.direction = Vector3(-0.707f, -0.707f, 0.0f);
	spData_.intensity = 12.5f;
	spData_.decay = 3.0f;
	spData_.cosAngle = 0.3f;
	spData_.cosFalloffStart = 0.5f;
	// 初期更新
	drLight_->Update(drData_);
	spLight_->Update(spData_);
	ptLight_->Update(ptData_);
}

void LightingManager::Update()
{
	drLight_->Update(drData_);
	spLight_->Update(spData_);
	ptLight_->Update(ptData_);
}

void LightingManager::ImGuiDraw()
{
	// ライティング
	if (ImGui::BeginTabBar("Lighting"))
	{
		float defaultSpeed = 0.01f;
		if (ImGui::BeginTabItem("DirectionalLight"))
		{
			ImGui::ColorEdit4("Color", &drData_.color.x);
			ImGui::DragFloat3("Direction", &drData_.direction.x, defaultSpeed);
			drData_.direction = Vector3::Normalize(drData_.direction);
			ImGui::DragFloat("Intensity", &drData_.intensity, defaultSpeed);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("PointLight"))
		{
			ImGui::ColorEdit4("ptColor", &ptData_.color.x);
			ImGui::DragFloat("ptDecay", &ptData_.decay, defaultSpeed);
			ImGui::DragFloat("ptIntensity", &ptData_.intensity, defaultSpeed);
			ImGui::DragFloat("ptRadius", &ptData_.radius, defaultSpeed);
			ImGui::DragFloat3("ptPosition", &ptData_.position.x, defaultSpeed);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("SpotLight"))
		{
			ImGui::ColorEdit4("spColor", &spData_.color.x);
			ImGui::DragFloat("spDecay", &spData_.decay, defaultSpeed);
			ImGui::DragFloat("spIntensity", &spData_.intensity, defaultSpeed);
			ImGui::DragFloat("spCosAngle", &spData_.cosAngle, defaultSpeed);
			ImGui::DragFloat("spCosFalloffStart", &spData_.cosFalloffStart, defaultSpeed);
			ImGui::DragFloat("spDistance", &spData_.distance, defaultSpeed);
			ImGui::DragFloat3("spPosition", &spData_.position.x, defaultSpeed);
			ImGui::DragFloat3("spDirection", &spData_.direction.x, defaultSpeed);
			spData_.direction = Vector3::Normalize(spData_.direction);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

}
