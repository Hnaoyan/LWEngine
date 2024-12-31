#include "CPUParticleEmitter.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

void CPUParticleEmitter::Initialize(Model* model)
{
	InstancedGroup::Initialize(model);


}

void CPUParticleEmitter::Update()
{
	// 削除処理
	units_.erase(std::remove_if(units_.begin(), units_.end(), [](const std::unique_ptr<InstancedUnit>& obj) {
		return obj->IsDead();
		}), units_.end());

	// 基底更新
	InstancedGroup::Update();
}

void CPUParticleEmitter::Draw(ModelDrawDesc desc)
{
	if (isInvisible_) {
		return;
	}

	// デスク
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.material = material_.get();
	modelDesc.texture = texture_;

	// 描画
	ModelRenderer::InstancedDraw(desc.camera, modelDesc, lightDesc, this);
}
