#include "BulletCluster.h"
#include "IBullet.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/2D/TextureManager.h"

uint32_t BulletCluster::sSerialNumber = 0;

void BulletCluster::Initialize(Model* model)
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 基底クラス初期化
	InstancedGroup::Initialize(model);

	texture_ = TextureManager::GetInstance()->Load("Resources/Default/white2x2.png");
}

void BulletCluster::Update()
{
	// 死亡処理
	units_.erase(std::remove_if(units_.begin(), units_.end(), [](const std::unique_ptr<InstancedUnit>& obj) {
		return obj->IsDead();
		}), units_.end());
	// 基底クラス更新
	InstancedGroup::Update();
}

void BulletCluster::Draw(ModelDrawDesc desc)
{
	// 描画
	model_->InstancedDraw(desc, this->unitNum_, buffer_.GetSRVGPU(), texture_);
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

void BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct)
{
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<IBullet>();
	instance->transform_ = transform;
	// 速度
	static_cast<IBullet*>(instance.get())->SetVelocity(direct);
	static_cast<IBullet*>(instance.get())->Initialize();
	// リストにムーブ
	units_.push_back(std::move(instance));
}
