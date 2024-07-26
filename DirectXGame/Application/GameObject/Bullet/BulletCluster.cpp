#include "BulletCluster.h"
#include "IBullet.h"

uint32_t BulletCluster::sSerialNumber = 0;

void BulletCluster::Initialize(Model* model)
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 基底クラス初期化
	InstancedGroup::Initialize(model);
}

void BulletCluster::Update()
{
	// 基底クラス更新
	InstancedGroup::Update();
}

void BulletCluster::Draw(ModelDrawDesc desc)
{
	// 描画
	model_->InstancedDraw(desc, this->unitNum_, srvHandles_.second);
}

void BulletCluster::ImGuiDraw()
{

}

void BulletCluster::CollisionUpdate(CollisionManager* manager)
{
	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
		it != units_.end(); ++it) {
		manager;
	}

}

void BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct)
{
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<IBullet>();
	instance->transform_ = transform;
	// 速度
	static_cast<IBullet*>(instance.get())->SetVelocity(direct * 5.0f);
	static_cast<IBullet*>(instance.get())->Initialize();
	// リストにムーブ
	units_.push_back(std::move(instance));
}
