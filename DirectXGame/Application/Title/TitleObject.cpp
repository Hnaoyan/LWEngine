#include "TitleObject.h"
#include "imgui.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/LwEnginePaths.h"

void TitleObject::Initialize(Model* model)
{
	IGameObject::Initialize(model);

	material_ = std::make_unique<Material>();
	material_->CreateMaterial();
	material_->color_.w = 0.75f;
	material_->discardThreshold_ = 0.1f;

	e_AnimState_ = AnimationType::Normally;

}

void TitleObject::Update()
{
	switch (e_AnimState_)
	{
		// 通常の状態
	case TitleObject::AnimationType::Normally:
		NormallyUpdate();
		break;
		// 開始の処理
	case TitleObject::AnimationType::Starting:
		StartingUpdate();
		break;
	default:
		break;
	}
	float turnAngle = 6.28f;
	// 一周を超えたら
	if (worldTransform_.transform_.rotate.y >= turnAngle) {
		worldTransform_.transform_.rotate.y -= turnAngle;
	}
	// マイナス値にならないように
	else if (worldTransform_.transform_.rotate.y <= 0.0f) {
		worldTransform_.transform_.rotate.y += turnAngle;
	}
	IGameObject::Update();
}

void TitleObject::Draw(ModelDrawDesc desc)
{
	this->material_->Update();

	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &worldTransform_;
	modelDesc.material = material_.get();

	// 本体の描画
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);
}

void TitleObject::ImGuiDraw()
{
	ImGui::Begin("PlObject");
	static float dragValue = 0.01f;
	ImGui::DragFloat("DragV", &dragValue, 0.1f);
	ImGui::DragFloat3("Pos", &worldTransform_.transform_.translate.x, dragValue);
	ImGui::DragFloat3("Rot", &worldTransform_.transform_.rotate.x, dragValue);
	ImGui::Checkbox("IsRotate", &isRotate_);
	ImGui::End();
}

void TitleObject::NormallyInitialize()
{
}

void TitleObject::NormallyUpdate()
{
	if (isRotate_) {
		worldTransform_.transform_.rotate.y += (1.0f / 90.0f);
	}
}

void TitleObject::StartingInitialize()
{
	startEulerY_ = worldTransform_.transform_.rotate.y;
}

void TitleObject::StartingUpdate()
{
	worldTransform_.transform_.rotate.y = Ease::Easing<float>(startEulerY_, 0.0f, 0.0f);
}
