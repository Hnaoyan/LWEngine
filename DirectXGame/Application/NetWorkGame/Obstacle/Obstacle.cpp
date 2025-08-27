#include "Obstacle.h"
#include "Engine/Collision/2D/Collider2DLists.h"
#include "Application/Collision/ColliderFilter.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/2D/TextureManager.h"
#include <imgui.h>

int Obstacle::sSerialNumber = 0;

void Obstacle::Initialize(Model* model)
{
	// 名前
	name_ = "Obstacle" + std::to_string(sSerialNumber);
	// シリアル番号更新
	sSerialNumber++;

	IGameObject2D::Initialize(model);

	Rectangle2D* rect = new Rectangle2D();
	rect->radius_ = 1.0f;
	rect->Initialize(this);
	rect->SetAttribute(kCollisionAttributeTerrain);
	rect->SetMask(kCollisionAttributePlayer);
	Collider2DShape* shape = new Collider2DShape();
	*shape = *rect;
	collider_.reset(shape);

	worldTransform_.transform_.translate = { 5.0f,0.0f,0.0f };

	//size_ = Vector3(1.0f, 1.0f, 1.0f);
	texture_ = model->GetModelData()->material.textureHandle;
	texture_ = TextureManager::GetInstance()->Load("Resources/default/white2x2.png");
}

void Obstacle::Update()
{
	size_ = worldTransform_.transform_.scale;
	IGameObject2D::Update();
}

void Obstacle::Draw(ModelDrawDesc desc)
{
	// マテリアル更新
	material_->Update();
	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &worldTransform_;
	modelDesc.material = material_.get();
	modelDesc.texture = texture_;
	// 描画
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);
}

void Obstacle::ImGuiDraw()
{
	//ImGui::Begin(name_.c_str());
	ImGui::SeparatorText(name_.c_str());

	std::string name = name_ + "Position";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.translate.x, 0.01f);

	name = name_ + "Size";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.scale.x, 0.01f);
	//ImGui::End();
}

void Obstacle::OnCollision([[maybe_unused]] ColliderObject target)
{
}

Rectangle2D* Obstacle::GetCollider()
{
	return std::get_if<Rectangle2D>(collider_.get());
}
