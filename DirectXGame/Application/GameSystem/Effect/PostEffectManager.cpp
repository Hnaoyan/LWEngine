#include "PostEffectManager.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "../GameSystem.h"
#include <imgui.h>

DashBlur PostEffectManager::sDashEffect;
DamageVignette PostEffectManager::sDamageEffect;

PostEffectManager::PostEffectManager()
{
	// ImGui
	InitializeGlobalValue();
	GlobalVariables* instance = GlobalVariables::GetInstance();
	// ブルーム
	bloomData_.sigma = 1.5f;
	bloomData_.threshold = 0.75f;
	// ブラー
	sDashEffect.data.centerPoint = { 0.5f,0.5f };
	sDashEffect.data.samplesNum = 4;
	// ビネット
	sDamageEffect.data.color = instance->GetValue<Vector3>("PostEffect", "VigColor");
	sDamageEffect.data.powValue = instance->GetValue<float>("PostEffect", "VigPow");
	sDamageEffect.data.scale = instance->GetValue<float>("PostEffect", "VigScale");

}

void PostEffectManager::Update()
{
	// ダメージとダッシュの両方
	if (sDashEffect.timer.IsActive() && sDamageEffect.timer.IsActive()) {
		sDashEffect.Update();
		sDamageEffect.Update();
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kVignetteBlur;
	}
	// ダッシュ
	else if (sDashEffect.timer.IsActive())
	{
		sDashEffect.Update();
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
	}
	// ダメージ
	else if (sDamageEffect.timer.IsActive())
	{
		sDamageEffect.Update();
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kVignette;
	}
	// 通常状態
	else
	{
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
	}

	// 値の渡す処理
	PostEffectRender::PostEffectDesc desc{};
	desc.bloom = bloomData_;
	desc.blur = sDashEffect.data;
	desc.vignette = sDamageEffect.data;
	PostEffectRender::GetInstance()->Update(desc);

}

void PostEffectManager::ImGuiDraw()
{
	// ビネット
	ImGui::DragFloat3("VignetteColor", &sDamageEffect.data.color.x, 0.01f);
	ImGui::DragFloat("VignettePow", &sDamageEffect.data.powValue, 0.01f);
	ImGui::DragFloat("VignetteScale", &sDamageEffect.data.scale, 0.01f);
}

void PostEffectManager::InitializeGlobalValue()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	std::string groupName = "PostEffect";
	instance->CreateGroup(groupName);
	// ビネット
	instance->AddValue(groupName, "VigColor", Vector3(0.5f, 0.0f, 0.0f));
	instance->AddValue(groupName, "VigPow", float(0.5f));
	instance->AddValue(groupName, "VigScale", float(10.0f));
	// ブルーム
	instance->AddValue(groupName, "BloomSigma", float(1.5f));
	instance->AddValue(groupName, "BloomThreshold", float(0.75f));
	// ブラー

}
