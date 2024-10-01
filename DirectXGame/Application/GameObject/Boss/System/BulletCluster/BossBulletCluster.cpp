#include "BossBulletCluster.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelManager.h"
#include "Engine/3D/ModelRenderer.h"
#include "Application/GameObject/Particle/User/ParticleLists.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameObject/Boss/System/Bullet/BossBullet.h"

void BossSystemContext::BulletCluster::Initialize(Model* model) {
	// 基底クラス初期化
	InstancedGroup::Initialize(model);
	globalVariables_ = GlobalVariables::GetInstance();
	texture_ = TextureManager::GetInstance()->Load("Resources/default/red2x2.png");
}

void BossSystemContext::BulletCluster::Update() {
	// 死亡処理
	units_.erase(std::remove_if(units_.begin(), units_.end(), [&](const std::unique_ptr<InstancedUnit>& obj) {
		// オブジェクト自体の削除（弾
		if (obj->IsDead()) {
			if (auto bullet = dynamic_cast<IBullet*>(obj.get())) {
				BossParticle::BulletEffect* effect = static_cast<BossParticle::BulletEffect*>(gpuParticle_->FindEmitter(bullet->GetTag()));
				// パーティクルの削除タイマー起動
				effect->StartTimer(60.0f);
			}
			return true;
		}
		return obj->IsDead();
		}), units_.end());
	// 基底クラス更新
	InstancedGroup::Update();
}

void BossSystemContext::BulletCluster::Draw(ModelDrawDesc desc) {
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
}

void BossSystemContext::BulletCluster::ImGuiDraw() {

}

void BossSystemContext::BulletCluster::CollisionUpdate(CollisionManager* manager) {
	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
		it != units_.end(); ++it) {
		manager->ListRegist(static_cast<IBullet*>((*it).get())->GetCollider());
	}
}

void BossSystemContext::BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct) {
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<NormalBullet>();
	// 速度
	static_cast<NormalBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * 5.0f);
	static_cast<NormalBullet*>(instance.get())->Initialize();
	instance->transform_ = transform;
	instance->transform_.scale = globalVariables_->GetValue<Vector3>("BossNormalBullet", "Scale");
	instance->Update();

	// 移動のパーティクル
	std::unique_ptr<ParticleEmitter> emitter = std::make_unique<BossParticle::BulletEffect>();
	emitter->Initialize(ModelManager::GetModel("Plane"));
	emitter->Update();
	BossParticle::BulletEffect* pre = static_cast<BossParticle::BulletEffect*>(emitter.get());
	pre->SetBullet(instance.get());
	pre->SetGPUParticleSystem(gpuParticle_);
	gpuParticle_->CreateEmitter(std::move(emitter), static_cast<IBullet*>(instance.get())->GetTag());
	parentTag_ = static_cast<IBullet*>(instance.get())->GetTag();

	// リストにムーブ
	units_.push_back(std::move(instance));
}

void BossSystemContext::BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct, float speed) {
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<NormalBullet>();
	// 速度
	static_cast<NormalBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * speed);
	static_cast<NormalBullet*>(instance.get())->Initialize();
	instance->transform_ = transform;
	instance->transform_.scale = globalVariables_->GetValue<Vector3>("BossNormalBullet", "Scale");
	instance->Update();

	// 移動のパーティクル
	std::unique_ptr<ParticleEmitter> emitter = std::make_unique<BossParticle::BulletEffect>();
	emitter->Initialize(ModelManager::GetModel("Plane"));
	emitter->Update();
	BossParticle::BulletEffect* pre = static_cast<BossParticle::BulletEffect*>(emitter.get());
	pre->SetBullet(instance.get());
	pre->SetGPUParticleSystem(gpuParticle_);
	gpuParticle_->CreateEmitter(std::move(emitter), static_cast<IBullet*>(instance.get())->GetTag());

	// リストにムーブ
	units_.push_back(std::move(instance));
}

void BossSystemContext::BulletCluster::AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player)
{
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<TrackingBullet>();
	// 速度
	static_cast<TrackingBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * speed);
	static_cast<TrackingBullet*>(instance.get())->SetPlayer(player);
	static_cast<TrackingBullet*>(instance.get())->SetBossPtr(boss_);
	static_cast<TrackingBullet*>(instance.get())->Initialize();
	instance->transform_ = transform;
	instance->transform_.scale = globalVariables_->GetValue<Vector3>("BossTrackingBullet", "Scale");
	instance->Update();

	// 移動のパーティクル
	std::unique_ptr<ParticleEmitter> emitter = std::make_unique<BossParticle::BulletEffect>();
	BossParticle::BulletEffect* pre = static_cast<BossParticle::BulletEffect*>(emitter.get());
	pre->SetBullet(instance.get());
	emitter->Initialize(ModelManager::GetModel("Plane"));
	emitter->Update();
	pre->SetGPUParticleSystem(gpuParticle_);
	gpuParticle_->CreateEmitter(std::move(emitter), static_cast<IBullet*>(instance.get())->GetTag());

	// リストにムーブ
	units_.push_back(std::move(instance));
}

void BossSystemContext::BulletCluster::AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player, TrackType type)
{
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<TrackingBullet>();
	// 速度
	static_cast<TrackingBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * speed);
	static_cast<TrackingBullet*>(instance.get())->SetPlayer(player);
	static_cast<TrackingBullet*>(instance.get())->SetBossPtr(boss_);
	static_cast<TrackingBullet*>(instance.get())->Initialize();
	static_cast<TrackingBullet*>(instance.get())->SetTrackType(type);
	instance->transform_ = transform;
	instance->transform_.scale = globalVariables_->GetValue<Vector3>("BossTrackingBullet", "Scale");
	instance->Update();

	// 移動のパーティクル
	std::unique_ptr<ParticleEmitter> emitter = std::make_unique<BossParticle::BulletEffect>();
	BossParticle::BulletEffect* pre = static_cast<BossParticle::BulletEffect*>(emitter.get());
	pre->SetBullet(instance.get());
	emitter->Initialize(ModelManager::GetModel("Plane"));
	emitter->Update();
	pre->SetGPUParticleSystem(gpuParticle_);
	gpuParticle_->CreateEmitter(std::move(emitter), static_cast<IBullet*>(instance.get())->GetTag());

	// リストにムーブ
	units_.push_back(std::move(instance));
}

int BossSystemContext::BulletCluster::BeginBulletPosBuffer()
{
	if (units_.size() != 0) {
		IBullet* bullet = static_cast<IBullet*>(units_.begin()->get());
		return (int)bullet->posBuffer_.size();
	}
	return 0;
}
