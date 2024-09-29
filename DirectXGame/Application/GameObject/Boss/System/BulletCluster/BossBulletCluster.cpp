//#include "BossBulletCluster.h"
//#include "Application/GameObject/GameObjectLists.h"
//#include "Engine/2D/TextureManager.h"
//#include "Engine/3D/ModelManager.h"
//#include "Engine/Particle/GPUParticleSystem.h"
//#include "Engine/Collision/CollisionManager.h"
//#include "../../Boss.h"
//
//void BossSystemContext::BulletCluster::Initialize(Model* model) {
//	// 基底クラス初期化
//	InstancedGroup::Initialize(model);
//	material_->color_ = { 1.0f,0.0f,0.0f,0.85f };
//	globalVariables_ = GlobalVariables::GetInstance();
//	texture_ = TextureManager::GetInstance()->Load("Resources/default/white2x2.png");
//}
//
//void BossSystemContext::BulletCluster::Update() {
//	// 死亡処理
//	units_.erase(std::remove_if(units_.begin(), units_.end(), [&](const std::unique_ptr<InstancedUnit>& obj) {
//		if (obj->IsDead()) {
//			if (auto bullet = dynamic_cast<IBullet*>(obj.get())) {
//				gpuParticle_->DeleteEmitter(bullet->GetTag());
//			}
//			return true;
//		}
//		return obj->IsDead();
//		}), units_.end());
//	// 基底クラス更新
//	InstancedGroup::Update();
//}
//
//void BossSystemContext::BulletCluster::Draw(ModelDrawDesc desc) {
//	// 描画
//	InstancedGroup::Draw(desc);
//}
//
//void BossSystemContext::BulletCluster::ImGuiDraw() {
//
//}
//
//void BossSystemContext::BulletCluster::CollisionUpdate(CollisionManager* manager) {
//	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
//		it != units_.end(); ++it) {
//		manager->ListRegist(static_cast<IBullet*>((*it).get())->GetCollider());
//	}
//}
//
//void BossSystemContext::BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct) {
//	// インスタンス作成
//	std::unique_ptr<InstancedUnit> instance = std::make_unique<NormalBullet>();
//	// 速度
//	static_cast<NormalBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * 5.0f);
//	static_cast<NormalBullet*>(instance.get())->Initialize();
//	instance->transform_ = transform;
//	instance->transform_.scale = globalVariables_->GetValue<Vector3>("BossNormalBullet", "Scale");
//	instance->Update();
//
//	// 移動のパーティクル
//	std::unique_ptr<ParticleEmitter> emitter = std::make_unique<BossParticle::BulletEffect>();
//	emitter->Initialize(ModelManager::GetModel("Plane"));
//	emitter->Update();
//	BossParticle::BulletEffect* pre = static_cast<BossParticle::BulletEffect*>(emitter.get());
//	pre->SetBullet(instance.get());
//	gpuParticle_->CreateEmitter(std::move(emitter), static_cast<IBullet*>(instance.get())->GetTag());
//
//	// リストにムーブ
//	units_.push_back(std::move(instance));
//}
//
//void BossSystemContext::BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct, float speed) {
//	// インスタンス作成
//	std::unique_ptr<InstancedUnit> instance = std::make_unique<NormalBullet>();
//	// 速度
//	static_cast<NormalBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * speed);
//	static_cast<NormalBullet*>(instance.get())->Initialize();
//	instance->transform_ = transform;
//	instance->transform_.scale = globalVariables_->GetValue<Vector3>("BossNormalBullet", "Scale");
//	instance->Update();
//
//	// 移動のパーティクル
//	std::unique_ptr<ParticleEmitter> emitter = std::make_unique<BossParticle::BulletEffect>();
//	emitter->Initialize(ModelManager::GetModel("Plane"));
//	emitter->Update();
//	BossParticle::BulletEffect* pre = static_cast<BossParticle::BulletEffect*>(emitter.get());
//	pre->SetBullet(instance.get());
//	gpuParticle_->CreateEmitter(std::move(emitter), static_cast<IBullet*>(instance.get())->GetTag());
//
//	// リストにムーブ
//	units_.push_back(std::move(instance));
//}
//
//void BossSystemContext::BulletCluster::AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player)
//{
//	// インスタンス作成
//	std::unique_ptr<InstancedUnit> instance = std::make_unique<TrackingBullet>();
//	// 速度
//	static_cast<TrackingBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * speed);
//	static_cast<TrackingBullet*>(instance.get())->SetPlayer(player);
//	static_cast<TrackingBullet*>(instance.get())->SetBossPtr(boss_);
//	static_cast<TrackingBullet*>(instance.get())->Initialize();
//	instance->transform_ = transform;
//	instance->transform_.scale = globalVariables_->GetValue<Vector3>("BossTrackingBullet", "Scale");
//	instance->Update();
//
//	// 移動のパーティクル
//	std::unique_ptr<ParticleEmitter> emitter = std::make_unique<BossParticle::BulletEffect>();
//	BossParticle::BulletEffect* pre = static_cast<BossParticle::BulletEffect*>(emitter.get());
//	pre->SetBullet(instance.get());
//	emitter->Initialize(ModelManager::GetModel("Plane"));
//	emitter->Update();
//	gpuParticle_->CreateEmitter(std::move(emitter), static_cast<IBullet*>(instance.get())->GetTag());
//
//	// リストにムーブ
//	units_.push_back(std::move(instance));
//}
//void BossSystemContext::BulletCluster::AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player, TrackType type)
//{
//	// インスタンス作成
//	std::unique_ptr<InstancedUnit> instance = std::make_unique<TrackingBullet>();
//	// 速度
//	static_cast<TrackingBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * speed);
//	static_cast<TrackingBullet*>(instance.get())->SetPlayer(player);
//	static_cast<TrackingBullet*>(instance.get())->SetBossPtr(boss_);
//	static_cast<TrackingBullet*>(instance.get())->Initialize();
//	static_cast<TrackingBullet*>(instance.get())->SetTrackType(type);
//	instance->transform_ = transform;
//	instance->transform_.scale = globalVariables_->GetValue<Vector3>("BossTrackingBullet", "Scale");
//	instance->Update();
//
//	// 移動のパーティクル
//	std::unique_ptr<ParticleEmitter> emitter = std::make_unique<BossParticle::BulletEffect>();
//	BossParticle::BulletEffect* pre = static_cast<BossParticle::BulletEffect*>(emitter.get());
//	pre->SetBullet(instance.get());
//	emitter->Initialize(ModelManager::GetModel("Plane"));
//	emitter->Update();
//	gpuParticle_->CreateEmitter(std::move(emitter), static_cast<IBullet*>(instance.get())->GetTag());
//
//	// リストにムーブ
//	units_.push_back(std::move(instance));
//}
