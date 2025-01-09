#include "PostEffectManager.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "../GameSystem.h"
#include <imgui.h>

DashBlur PostEffectManager::sDashEffect;
DamageVignette PostEffectManager::sDamageEffect;
SlowGrayScale PostEffectManager::sSlowEffect;

PostEffectManager::PostEffectManager()
{
	// ImGui
	InitializeGlobalValue();
	GlobalVariables* instance = GlobalVariables::GetInstance();
	// ブルーム
	float sigma = 1.5f;	// シグマの値
	float threshold = 0.75f;	// 閾値
	bloomData_.sigma = sigma;
	bloomData_.threshold = threshold;
	// ブラー
	Vector2 center = { 0.5f,0.5f };	// センターの値
	sDashEffect.data.centerPoint = center;
	sDashEffect.data.samplesNum = instance->GetValue<int32_t>("PostEffect", "BlurSampleNum");	// サンプル数;
	sDashEffect.maxWidth = instance->GetValue<float>("PostEffect", "BlurWidth");
	// ビネット
	sDamageEffect.data.color = instance->GetValue<Vector3>("PostEffect", "VigColor");
	sDamageEffect.data.powValue = instance->GetValue<float>("PostEffect", "VigPow");
	sDamageEffect.data.scale = instance->GetValue<float>("PostEffect", "VigScale");
}

void PostEffectManager::Initialize(GameSystem* system)
{
	// ゲームシステム設定
	gameSystem_ = system;
}

void PostEffectManager::Update()
{
#ifdef IMGUI_ENABLED
	GlobalVariables* instance = GlobalVariables::GetInstance();
	// ブラー
	sDashEffect.data.samplesNum = instance->GetValue<int32_t>("PostEffect", "BlurSampleNum");	// サンプル数;
	sDashEffect.maxWidth = instance->GetValue<float>("PostEffect", "BlurWidth");
	// ビネット
	sDamageEffect.data.color = instance->GetValue<Vector3>("PostEffect", "VigColor");
	sDamageEffect.data.powValue = instance->GetValue<float>("PostEffect", "VigPow");
	sDamageEffect.data.scale = instance->GetValue<float>("PostEffect", "VigScale");
#endif // IMGUI_ENABLED


	// リプレイ中なら
	if (gameSystem_->GetReplayManager()->IsReplayNow()) {
		return;
	}

	sSlowEffect.Update();
	sDashEffect.Update();
	sDamageEffect.Update();
	// スローモーション
	if (sSlowEffect.timer.IsActive()) {
		nowEffect = Pipeline::PostEffectType::kGrayscaleBloom;
	}
	// ダメージとダッシュの両方
	else if (sDashEffect.timer.IsActive() && sDamageEffect.timer.IsActive()) {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kVignetteBlur;
	}
	// ダッシュ
	else if (sDashEffect.timer.IsActive())
	{
		nowEffect = Pipeline::PostEffectType::kRadialBlur;
	}
	// ダメージ
	else if (sDamageEffect.timer.IsActive())
	{
		nowEffect = Pipeline::PostEffectType::kVignette;
	}
	// 通常状態
	else
	{
		nowEffect = Pipeline::PostEffectType::kBloom;
	}
	// 現在のエフェクトの種類設定
	PostEffectRender::sPostEffect = nowEffect;

	// 値の渡す処理
	PostEffectRender::PostEffectDesc desc{};
	desc.bloom = bloomData_;
	desc.blur = sDashEffect.data;
	desc.vignette = sDamageEffect.data;
	desc.grayscale = sSlowEffect.data;
	PostEffectRender::GetInstance()->Update(desc);
}

void PostEffectManager::ImGuiDraw()
{
	if (ImGui::TreeNode("PostEffectType")) {
		if (ImGui::Button("Bloom")) {
			nowEffect = Pipeline::PostEffectType::kBloom;
		}
		if (ImGui::Button("Vignette")) {
			nowEffect = Pipeline::PostEffectType::kVignette;
		}
		if (ImGui::Button("VignetteBlur")) {
			nowEffect = Pipeline::PostEffectType::kVignetteBlur;
		}
		if (ImGui::Button("Gray")) {
			nowEffect = Pipeline::PostEffectType::kGrayScale;
		}
		if (ImGui::Button("GrayBloom")) {
			nowEffect = Pipeline::PostEffectType::kGrayscaleBloom;
		}
		ImGui::TreePop();
	}
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
	instance->AddValue(groupName, "BlurWidth", float(0.15f));
	instance->AddValue(groupName, "BlurSampleNum", int32_t(4));
}
