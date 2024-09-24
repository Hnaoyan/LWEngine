#include "SkyDomeObject.h"
#include "imgui.h"

#include "Engine/3D/ModelRenderer.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void SkyDomeObject::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.transform_.scale = { 500.0f,500.0f,500.0f };
	worldTransform_.UpdateMatrix();
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();

	// 外部書き出しの初期化
	GlobalValueInitialize();
}

void SkyDomeObject::Update()
{
#ifdef _DEBUG
	GlobalValueUpdate();
#endif // _DEBUG

	// マテリアル
	material_->Update();
	// 行列更新
	worldTransform_.UpdateMatrix();
}

void SkyDomeObject::ImGuiDraw()
{
	ImGui::Begin("SkyDomeObject");
	float scale = worldTransform_.transform_.scale.x;
	ImGui::DragFloat("Scale", &scale, 0.1f);
	worldTransform_.transform_.scale = { scale,scale,scale };
	worldTransform_.UpdateMatrix();
	ImGui::ColorEdit4("Color", &material_->color_.x);
	ImGui::End();
}

void SkyDomeObject::Draw(ModelDrawDesc desc)
{
	// マテリアル更新
	model_->GetMaterial()->Update();
	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &worldTransform_;
	modelDesc.material = material_.get();
	// プレイヤーの描画
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);

}

void SkyDomeObject::GlobalValueInitialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "Skydome";
	// 生成
	global->CreateGroup(groupName);
	global->AddValue(groupName, "Translate", worldTransform_.transform_.translate);
	global->AddValue(groupName, "Scale", worldTransform_.transform_.scale);
	global->AddValue(groupName, "MaterialColor", material_->color_);
	// 更新
	GlobalValueUpdate();
}

void SkyDomeObject::GlobalValueUpdate()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "Skydome";
	
	worldTransform_.transform_.translate = global->GetValue<Vector3>(groupName, "Translate");
	worldTransform_.transform_.scale = global->GetValue<Vector3>(groupName, "Scale");
	material_->color_ = global->GetValue<Vector4>(groupName, "MaterialColor");
}
