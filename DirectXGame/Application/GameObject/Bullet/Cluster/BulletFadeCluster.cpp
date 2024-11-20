#include "BulletFadeCluster.h"
#include "Application/GameObject/Particle/User/Trail/TrailManager.h"

void BulletFadeCluster::Initialize(Model* model)
{
	IBulletCluster::Initialize(model);
}

void BulletFadeCluster::Update()
{
	IBulletCluster::Update();

	if (units_.size() == 0) {
		// 削除処理にする
		deleteRequest_ = true;
	}

}

void BulletFadeCluster::Draw(ModelDrawDesc desc)
{
	IBulletCluster::Draw(desc);
}
