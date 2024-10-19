#include "TrailManager.h"
#include "Application/GameObject/Bullet/Trail/BulletTrail.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

TrailManager::TrailManager()
{

}

void TrailManager::Draw(ICamera* camera)
{
	// 消す処理
	trails_.erase(std::remove_if(trails_.begin(), trails_.end(), [](const std::unique_ptr<BulletTrail>& obj) {
		return obj->IsDelete();
		}), trails_.end());

	for (auto it = trails_.begin(); it != trails_.end(); ++it) {
		(*it)->Update(camera);
		ModelRenderer::TrailDraw(camera, (*it)->polygon_.get());
	}
}

void TrailManager::AddTrail(std::unique_ptr<BulletTrail> trail)
{
	trails_.push_back(std::move(trail));
}
