#include "BossBullet.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

uint32_t BossSystemContext::IBullet::sSerialNumber = 0;

void BossSystemContext::IBullet::BufferUpdate()
{
	// バッファーに追加
	posBuffer_.push_back(transform_.translate);

	// 上限値により頭を削除
	int deleteSize = 75;
	if ((int)posBuffer_.size() > deleteSize) {
		posBuffer_.erase(posBuffer_.begin());
	}
}

void BossSystemContext::IBullet::DrawTrail(ICamera* camera)
{
	// 軌跡クラスの設定
	trail_->Draw(camera);
	// 軌跡の描画
	ModelRenderer::TriangleDraw(camera, trail_->triangle_.get());
}
