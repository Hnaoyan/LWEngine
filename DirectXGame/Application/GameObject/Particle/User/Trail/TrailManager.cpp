#include "TrailManager.h"
#include "Application/GameObject/Bullet/Trail/BulletTrail.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/Particle/GPUParticleSystem.h"

TrailManager::TrailManager()
{

}

void TrailManager::Draw(ICamera* camera)
{
	// 消す処理
	trails_.erase(std::remove_if(trails_.begin(), trails_.end(), [&](const std::unique_ptr<BulletTrail>& obj) {
		// 死ぬ瞬間
		if (obj->IsDelete()) {
			gpuParticle_->DeleteEmitter(obj->GetTag());
			std::string name = obj->GetTag() + "Break";
			gpuParticle_->DeleteEmitter(name);
		}
		return obj->IsDelete();
		}), trails_.end());

	// 描画と更新
	for (auto it = trails_.begin(); it != trails_.end(); ++it) {
		// 更新
		(*it)->Update(camera);

		if ((*it)->GetAttribute() == TrackingAttribute::kSuperior && isInvisibles_[uint32_t(TrackingAttribute::kSuperior)])
		{
			continue;
		}
		if ((*it)->GetAttribute() == TrackingAttribute::kInferior && isInvisibles_[uint32_t(TrackingAttribute::kInferior)])
		{
			continue;
		}
		if ((*it)->GetAttribute() == TrackingAttribute::kGenius && isInvisibles_[uint32_t(TrackingAttribute::kGenius)])
		{
			continue;
		}
		// 描画
		ModelRenderer::TrailDraw(camera, (*it)->polygon_.get());
	}
}

void TrailManager::AddTrail(std::unique_ptr<BulletTrail> trail)
{
	trails_.push_back(std::move(trail));
}

BulletTrail* TrailManager::FindTrail(const std::string& name)
{
	for (std::vector<std::unique_ptr<BulletTrail>>::iterator it = trails_.begin(); it != trails_.end(); ++it) {
		// 名前が一致した場合
		if ((*it)->GetTag() == name) {
			return (*it).get();
		}
	}
	// なかった場合
	return nullptr;
}