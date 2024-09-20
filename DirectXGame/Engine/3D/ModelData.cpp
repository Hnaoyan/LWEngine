#include "ModelData.h"
#include "Engine/3D/Drawer/Model.h"
#include <cassert>

void DrawDesc::ModelDesc::SetDesc(Model* model)
{
	mesh = model->GetMesh();
	material = model->GetMaterial();
	modelData = model->GetModelData();
	texture = model->GetModelData()->material.textureHandle;
}

void DrawDesc::LightDesc::Draw(ID3D12GraphicsCommandList* cmdList)
{
	assert(cmdList);
	// 平行光源
	directionalLight->Draw(cmdList, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kDirectionalLight));
	// 方向光源
	spotLight->Draw(cmdList, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kSpotLight));
	// 点光源
	pointLight->Draw(cmdList, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kPointLight));
}
