#include "BulletTrail.h"

BulletTrail::BulletTrail()
{
	maxLength = 50;
	triangle_ = std::make_unique<Triangle3D>();
	triangle_->Initialize();
}

void BulletTrail::UpdateTrail(const Vector3& newPoint)
{
	// 要素を追加
	trailPoints_.push_back(newPoint);
	// 最大値を超えた場合古いのを削除
	if (trailPoints_.size() > maxLength) {
		trailPoints_.erase(trailPoints_.begin());
	}
}

void BulletTrail::Draw(ICamera* camera)
{
	if (!triangle_->IsCamera()) {
		triangle_->SetCamera(camera);
	}
	triangle_->Update(trailPoints_);
	
}
