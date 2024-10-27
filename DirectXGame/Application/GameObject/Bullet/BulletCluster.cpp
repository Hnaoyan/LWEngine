#include "BulletCluster.h"
#include "IBullet.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Application/GameObject/Particle/User/Trail/TrailManager.h"
#include "Application/GameObject/Particle/User/ParticleLists.h"
#include "Application/GameObject/Particle/User/Bullet/CPUEffect/BulletBombCluster.h"

uint32_t BulletCluster::sSerialNumber = 0;

void BulletCluster::Initialize(Model* model)
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 基底クラス初期化
	InstancedGroup::Initialize(model);

	texture_ = TextureManager::GetInstance()->Load("Resources/Default/white2x2.png");

	bombEffectCluster_ = std::make_unique<BulletBombCluster>();
	bombEffectCluster_->Initialize(ModelManager::GetModel("Plane"));
}

void BulletCluster::Update()
{
	// 死亡処理
	units_.erase(std::remove_if(units_.begin(), units_.end(), [&](const std::unique_ptr<InstancedUnit>& obj) {
		//if (obj->IsDead()) {
		//	gpuParticle_->DeleteEmitter(static_cast<IBullet*>(obj.get())->GetTag());
		//}
		// 壊れたパーティクル
		if (obj->IsDead()) {
			static_cast<BulletBombCluster*>(bombEffectCluster_.get())->BulletBomb(obj->GetWorldPosition());
		}
		return obj->IsDead();
		}), units_.end());

	bombEffectCluster_->Update();
	// 基底クラス更新
	InstancedGroup::Update();
}

void BulletCluster::Draw(ModelDrawDesc desc)
{
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
	ModelRenderer::InstancedDraw(desc.camera, modelDesc, lightDesc, this->unitNum_, buffer_.GetSRVGPU());

	// エフェクトの描画
	bombEffectCluster_->Draw(desc);
}

void BulletCluster::ImGuiDraw()
{

}

void BulletCluster::CollisionUpdate(CollisionManager* manager)
{
	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
		it != units_.end(); ++it) {
		manager->ListRegist(static_cast<IBullet*>((*it).get())->GetCollider());
	}
}

void BulletCluster::AddBullet(std::unique_ptr<IBullet> bullet)
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	// 軌跡の管理
	std::unique_ptr<BulletTrail> trailInstance = std::make_unique<BulletTrail>(bullet.get());
	// 移動のパーティクル
	std::unique_ptr<BulletParticle::MoveEffect> particle = std::make_unique<BulletParticle::MoveEffect>();
	// 壊れた時のパーティクル
	std::unique_ptr<BulletParticle::BreakEffect> breakPartice = std::make_unique<BulletParticle::BreakEffect>();

	// トレイル
	trailInstance->SetLength(global->GetValue<int32_t>("BossTrackingBullet", "TrailSaveFrame"));
	trailInstance->polygon_->SetMinWidth(global->GetValue<float>("BossTrackingBullet", "TrailMinWidth"));
	trailInstance->polygon_->SetMaxWidth(global->GetValue<float>("BossTrackingBullet", "TrailMaxWidth"));
	trailInstance->SetBulletTag(bullet->GetTag());

	// 弾
	bullet->SetTrail(trailInstance.get());
	bullet->SetBreakEmitter(breakPartice.get());

	// パーティクル
	particle->Initialize(ModelManager::GetModel("Plane"));
	particle->SetGPUParticleSystem(gpuParticle_);
	particle->SetBullet(bullet.get());
	particle->SetTrail(trailInstance.get());

	breakPartice->Initialize(ModelManager::GetModel("Plane"));
	breakPartice->SetGPUParticleSystem(gpuParticle_);

	// リストに追加
	trailManager_->AddTrail(std::move(trailInstance));	// 軌跡
	gpuParticle_->CreateEmitter(std::move(particle), bullet->GetTag()); // エミッター
	std::string name = bullet->GetTag() + "Break";
	gpuParticle_->CreateEmitter(std::move(breakPartice), name);
	units_.push_back(std::move(bullet));	// 弾
}
