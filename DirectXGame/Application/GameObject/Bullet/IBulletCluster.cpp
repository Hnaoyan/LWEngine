#include "IBulletCluster.h"
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
#include "Application/GameObject/Particle/User/Bullet/CPUEffect/BulletBombEffect.h"

uint32_t IBulletCluster::sSerialNumber = 0u;

void IBulletCluster::Initialize(Model* model)
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 基底クラス初期化
	InstancedGroup::Initialize(model);

	texture_ = TextureManager::GetInstance()->Load("Resources/Default/white2x2.png");
	//material_->color_ = { 0.0f,1.0f,0.0f,1.0f };

	//bombEffectCluster_ = std::make_unique<BulletBombCluster>();
	//bombEffectCluster_->Initialize(ModelManager::GetModel("Plane"));

	bulletFactory_ = std::make_unique<BulletFactory>();
}

void IBulletCluster::Update()
{
	// 死亡処理
	units_.erase(std::remove_if(units_.begin(), units_.end(), [&](const std::unique_ptr<InstancedUnit>& obj) {
		//// 壊れたパーティクル	
		//if (obj->IsDead()) {
		//	static_cast<BulletBombCluster*>(bombEffectCluster_.get())->BulletBomb(obj->GetWorldPosition());
		//}
		if (obj->IsDead()) {
			//bombCluster_->BulletBomb(obj->GetWorldPosition());
			bombCluster_->BombEffect(obj->GetWorldPosition());
		}
		return obj->IsDead();
		}), units_.end());

	//bombEffectCluster_->Update();
	// 基底クラス更新
	InstancedGroup::Update();
}

void IBulletCluster::Draw(ModelDrawDesc desc)
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
	ModelRenderer::InstancedDraw(desc.camera, modelDesc, lightDesc, this);
}

void IBulletCluster::CollisionUpdate(CollisionManager* manager)
{
	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
		it != units_.end(); ++it) {
		manager->ListRegist(static_cast<IBullet*>((*it).get())->GetCollider());
	}
}

void IBulletCluster::AddBullet(std::unique_ptr<IBullet> bullet)
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
	trailInstance->SetTrailColor(trailColor_);

	// 弾
	bullet->SetTrail(trailInstance.get());
	bullet->SetBreakEmitter(breakPartice.get());

	// パーティクル
	particle->Initialize(ModelManager::GetModel("Plane"));
	particle->SetGPUParticleSystem(gpuParticle_);
	particle->SetBullet(bullet.get());
	particle->SetTrail(trailInstance.get());

	// 敵のクラスターである場合
	size_t position = tag_.find(":");
	if (position != std::string::npos) {
		std::string zokusei = tag_.substr(0, position);
		if ("Boss" == zokusei) {
			particle->SetEmitPattern(5);
		}
	}

	breakPartice->Initialize(ModelManager::GetModel("Plane"));
	breakPartice->SetGPUParticleSystem(gpuParticle_);

	// リストに追加
	trailManager_->AddTrail(std::move(trailInstance));	// 軌跡
	gpuParticle_->CreateEmitter(std::move(particle), bullet->GetTag()); // エミッター
	std::string name = bullet->GetTag() + "Break";
	//gpuParticle_->CreateEmitter(std::move(breakPartice), name);
	units_.push_back(std::move(bullet));	// 弾
}

void IBulletCluster::AddBullet(const BulletBuilder& builder, BulletType type)
{
	GlobalVariables* global = GlobalVariables::GetInstance();

	std::unique_ptr<IBullet> bullet = builder.Build(type);
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
	trailInstance->SetTrailColor(trailColor_);

	// 弾
	bullet->SetTrail(trailInstance.get());
	bullet->SetBreakEmitter(breakPartice.get());

	// パーティクル
	particle->Initialize(ModelManager::GetModel("Plane"));
	particle->SetGPUParticleSystem(gpuParticle_);
	particle->SetBullet(bullet.get());
	particle->SetTrail(trailInstance.get());

	// 敵のクラスターである場合
	size_t position = tag_.find(":");
	if (position != std::string::npos) {
		std::string zokusei = tag_.substr(0, position);
		if ("Boss" == zokusei) {
			particle->SetEmitPattern(5);
		}
	}

	breakPartice->Initialize(ModelManager::GetModel("Plane"));
	breakPartice->SetGPUParticleSystem(gpuParticle_);

	// リストに追加
	trailManager_->AddTrail(std::move(trailInstance));	// 軌跡
	gpuParticle_->CreateEmitter(std::move(particle), bullet->GetTag()); // エミッター
	std::string name = bullet->GetTag() + "Break";
	units_.push_back(std::move(bullet));	// 弾
}
