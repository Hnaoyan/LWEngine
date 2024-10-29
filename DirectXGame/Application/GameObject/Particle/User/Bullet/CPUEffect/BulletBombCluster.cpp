#include "BulletBombCluster.h"
#include "BulletBombEffect.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

void BulletBombCluster::Initialize(Model* model)
{
	// 初期化
	InstancedGroup::Initialize(model);

	
	texture_ = TextureManager::Load("Resources/Effect/Frea.png");
	material_->color_ = { 0.5f,0.5f,0.5f,0.85f };
}

void BulletBombCluster::Update()
{
	// 削除処理
	units_.erase(std::remove_if(units_.begin(), units_.end(), [&](const std::unique_ptr<InstancedUnit>& obj) {
		return obj->IsDead();
		}), units_.end());
	// 更新
	InstancedGroup::Update();
}

void BulletBombCluster::Draw(ModelDrawDesc desc)
{

	for (auto it = units_.begin(); it != units_.end(); ++it) {
		if ((*it)->IsBillBoard()) {
			(*it)->transform_.rotate = desc.camera->transform_.rotate;
		}
	}

	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	material_->Update();
	modelDesc.material = material_.get();
	modelDesc.texture = texture_;
	// 描画
	ModelRenderer::InstancedDraw(desc.camera, modelDesc, lightDesc, this);
}

void BulletBombCluster::BulletBomb(const Vector3& position)
{
	std::unique_ptr<BulletBombEffect> instance = std::make_unique<BulletBombEffect>();
	instance->Initialize();
	instance->transform_.translate = position;
	AddParticle(std::move(instance));
}

void BulletBombCluster::AddParticle(std::unique_ptr<BulletBombEffect> particle)
{
	// プッシュ
	//particle->Initialize();
	units_.push_back(std::move(particle));
}

