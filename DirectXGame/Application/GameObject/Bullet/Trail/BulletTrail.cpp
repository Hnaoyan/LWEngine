#include "BulletTrail.h"
#include "Engine/LwLib/LwLibLists.h"
#include <algorithm>

BulletTrail::BulletTrail()
{
	maxLength = 50;
	// ポリゴン作成
	triangle_ = std::make_unique<Triangle3D>();
	triangle_->Initialize();
}

void BulletTrail::UpdateTrail(const Vector3& newPoint)
{
	// 要素を追加
	trailPoints_.push_back(newPoint);
	// 最大値を超えた場合古いのを削除
	// 時間で消えるようにするアルゴリズムを追加する
	if (trailPoints_.size() > maxLength) {
		trailPoints_.erase(trailPoints_.begin());
	}
}

void BulletTrail::Draw(ICamera* camera)
{
	// カメラのポインタを設定
	if (!triangle_->IsCamera()) {
		triangle_->SetCamera(camera);
	}
	// Catmull-Rom曲線を使った座標
	if (trailPoints_.size() > 8) {
		std::vector<Vector3> interpolatedPoints;
		for (int i = 1; i < trailPoints_.size() - 2; ++i) {
			for (float t = 0.0f; t <= 1.0f; t += (1.0f / 30.0f)) {
				t = std::clamp(t, 0.0f, 1.0f);
				interpolatedPoints.push_back(LwLib::Curve::CatmullRomSpline(trailPoints_[i - 1], trailPoints_[i], trailPoints_[i + 1], trailPoints_[i + 2], t));
			}
		}
		triangle_->Update(interpolatedPoints);
	}
	// 通常
	else {
		triangle_->Update(trailPoints_);
	}
}
