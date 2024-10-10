#include "BulletTrail.h"
#include "Engine/LwLib/LwLibLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include <algorithm>

BulletTrail::BulletTrail()
{
	// 外部読み込み
	GlobalValueInitialize();
	// ポリゴン作成
	polygon_ = std::make_unique<Trail3D>();
	//triangle_->Initialize();
}

BulletTrail::BulletTrail(InstancedUnit* unit)
{
	// 外部読み込み
	GlobalValueInitialize();
	// ポリゴン作成
	polygon_ = std::make_unique<Trail3D>();
	// ユニットのポインタ
	unit_.emplace(unit);
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
	if (!polygon_->IsCamera()) {
		polygon_->SetCamera(camera);
	}
	//// Catmull-Rom曲線を使った座標
	//if (trailPoints_.size() > 8) {
	//	std::vector<Vector3> interpolatedPoints;
	//	for (int i = 1; i < trailPoints_.size() - 2; ++i) {
	//		for (float t = 0.0f; t <= 1.0f; t += (1.0f / 30.0f)) {
	//			t = std::clamp(t, 0.0f, 1.0f);
	//			interpolatedPoints.push_back(LwLib::Curve::CatmullRomSpline(trailPoints_[i - 1], trailPoints_[i], trailPoints_[i + 1], trailPoints_[i + 2], t));
	//		}
	//	}
	//	polygon_->Update(interpolatedPoints);
	//}
	//// 通常
	//else {
	//	polygon_->Update(trailPoints_);
	//}
	polygon_->Update(trailPoints_);
}

void BulletTrail::GlobalValueInitialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	maxLength = global->GetValue<int32_t>("BossTrackingBullet", "TrailSaveFrame");
}
