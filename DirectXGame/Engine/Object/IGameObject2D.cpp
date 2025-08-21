#include "IGameObject2D.h"
#include "Engine/Collision/2D/Collider2DLists.h"

void IGameObject2D::Initialize(Model* model)
{
	model_ = model;
	// マテリアル
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();

	worldTransform_.Initialize();
}

void IGameObject2D::Update()
{
	// 座標
	worldTransform_.UpdateMatrix();
	// コライダー
	ColliderUpdate();
}

void IGameObject2D::ColliderUpdate()
{
	Rectangle2D* rect = std::get<Rectangle2D*>(*collider_);
	rect->Update(Vector2(worldTransform_.GetWorldPosition().x, worldTransform_.GetWorldPosition().y));

	Collider2DShape* shape = new Collider2DShape();
	*shape = rect;
	collider_.reset(shape);
}

void IGameObject2D::Draw(ModelDrawDesc desc)
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

	// 描画
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);
}