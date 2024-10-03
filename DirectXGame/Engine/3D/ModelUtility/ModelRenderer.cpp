#include "ModelRenderer.h"
#include "Engine/2D/TextureManager.h"
#include "../Material.h"
#include "../Mesh.h"
#include <cassert>

ID3D12GraphicsCommandList* ModelRenderer::sCommandList_ = nullptr;
GeneralPipeline ModelRenderer::sPipeline_;

void ModelRenderer::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	assert(cmdList);
	// コマンドリストの設定
	sCommandList_ = cmdList;
}

void ModelRenderer::PostDraw()
{
	// コマンドリストの解除
	sCommandList_ = nullptr;
}

void ModelRenderer::NormalDraw(ICamera* camera, const DrawDesc::ModelDesc& modelDesc, const DrawDesc::LightDesc& lightDesc)
{
	// プリミティブ形状の設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// パイプラインの設定
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kModel)]);

	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

	// ワールド行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::ModelRegister::kWorldTransform),
		modelDesc.worldTransform->GetCBuffer()->GetGPUVirtualAddress());
	// ビュープロジェクション行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::ModelRegister::kViewProjection),
		camera->GetCBuffer()->GetGPUVirtualAddress());

	//---メッシュの設定---//
	// 頂点バッファの設定
	sCommandList_->IASetVertexBuffers(0, 1, &modelDesc.mesh->vbView_);
	// インデックスバッファの設定
	sCommandList_->IASetIndexBuffer(&modelDesc.mesh->ibView_);

	//---マテリアルの設定---//
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(ModelRegister::kTexture), modelDesc.texture);
	// 環境マップ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(ModelRegister::kMapTexture), TextureManager::sEnvironmentTexture);
	// Dissolve
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(ModelRegister::kDissolveTexture), modelDesc.material->dissolveTexture_);

	// マテリアル
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kMaterial), modelDesc.material->buffer_.cBuffer->GetGPUVirtualAddress());

	// ライト
	// 平行光源
	lightDesc.directionalLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::ModelRegister::kDirectionalLight));
	// 方向光源
	lightDesc.spotLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::ModelRegister::kSpotLight));
	// 点光源
	lightDesc.pointLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::ModelRegister::kPointLight));

	// ドローコール
	sCommandList_->DrawIndexedInstanced(UINT(modelDesc.modelData->indices.size()), 1, 0, 0, 0);
}

void ModelRenderer::SkinningAnimationDraw(const ModelDrawDesc& desc, Animation* animation)
{
	// プリミティブ形状の設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// パイプラインの設定
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kSkinningModel)]);

	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

	// ワールド行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(SkinningModelRegister::kWorldTransform),
		desc.worldTransform->GetCBuffer()->GetGPUVirtualAddress());
	// ビュープロジェクション行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(SkinningModelRegister::kViewProjection),
		desc.camera->GetCBuffer()->GetGPUVirtualAddress());

	//---メッシュの設定---//
	// 頂点バッファの設定
	D3D12_VERTEX_BUFFER_VIEW vbvs[2] =
	{
		desc.mesh->vbView_,
		animation->skinCluster_.influenceBufferView
	};

	sCommandList_->IASetVertexBuffers(0, 2, vbvs);
	// インデックスバッファの設定
	sCommandList_->IASetIndexBuffer(&desc.mesh->ibView_);

	//---マテリアルの設定---//
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(SkinningModelRegister::kTexture), desc.texture);
	// 環境マップ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(SkinningModelRegister::kMapTexture), TextureManager::sEnvironmentTexture);
	// マテリアル
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(SkinningModelRegister::kMaterial), desc.material->buffer_.cBuffer->GetGPUVirtualAddress());

	// ライト
	if (desc.directionalLight) {
		desc.directionalLight->Draw(sCommandList_, static_cast<uint32_t>(SkinningModelRegister::kDirectionalLight));
	}
	if (desc.spotLight) {
		desc.spotLight->Draw(sCommandList_, static_cast<uint32_t>(SkinningModelRegister::kSpotLight));
	}
	if (desc.pointLight) {
		desc.pointLight->Draw(sCommandList_, static_cast<uint32_t>(SkinningModelRegister::kPointLight));
	}

	// ドローコール
	sCommandList_->DrawIndexedInstanced(UINT(desc.modelData->indices.size()), 1, 0, 0, 0);
}

void ModelRenderer::InstancedDraw(const ModelDrawDesc& desc, uint32_t instanceNum, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	// プリミティブ形状の設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// パイプラインの設定
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kInstancedModel)]);

	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

	// ビュープロジェクション行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::InstancedUnitRegister::kViewProjection),
		desc.camera->GetCBuffer()->GetGPUVirtualAddress());

	//---メッシュの設定---//
	// 頂点バッファの設定
	sCommandList_->IASetVertexBuffers(0, 1, &desc.mesh->vbView_);
	// インデックスバッファの設定
	sCommandList_->IASetIndexBuffer(&desc.mesh->ibView_);

	//---マテリアルの設定---//
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(Pipeline::InstancedUnitRegister::kTexture), desc.texture);
	// 環境マップ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(Pipeline::InstancedUnitRegister::kMapTexture), TextureManager::sEnvironmentTexture);
	// マテリアル
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::InstancedUnitRegister::kMaterial), desc.material->buffer_.cBuffer->GetGPUVirtualAddress());

	// トランスフォーム
	sCommandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(Pipeline::InstancedUnitRegister::kWorldTransform), handle);

	// ライト
	if (desc.directionalLight) {
		desc.directionalLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kDirectionalLight));
	}
	if (desc.spotLight) {
		desc.spotLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kSpotLight));
	}
	if (desc.pointLight) {
		desc.pointLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kPointLight));
	}

	// ドローコール
	sCommandList_->DrawIndexedInstanced(UINT(desc.modelData->indices.size()), instanceNum, 0, 0, 0);
}

void ModelRenderer::InstancedDraw(ICamera* camera, const DrawDesc::ModelDesc& modelDesc, const DrawDesc::LightDesc& lightDesc, uint32_t instanceNum, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	// プリミティブ形状の設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// パイプラインの設定
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kInstancedModel)]);

	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

	// ビュープロジェクション行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::InstancedUnitRegister::kViewProjection),
		camera->GetCBuffer()->GetGPUVirtualAddress());

	//---メッシュの設定---//
	// 頂点バッファの設定
	sCommandList_->IASetVertexBuffers(0, 1, &modelDesc.mesh->vbView_);
	// インデックスバッファの設定
	sCommandList_->IASetIndexBuffer(&modelDesc.mesh->ibView_);

	//---マテリアルの設定---//
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(Pipeline::InstancedUnitRegister::kTexture), modelDesc.texture);
	// 環境マップ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(Pipeline::InstancedUnitRegister::kMapTexture), TextureManager::sEnvironmentTexture);
	// マテリアル
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::InstancedUnitRegister::kMaterial), modelDesc.material->buffer_.cBuffer->GetGPUVirtualAddress());

	// トランスフォーム
	sCommandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(Pipeline::InstancedUnitRegister::kWorldTransform), handle);

	// ライト
	// 平行光源
	lightDesc.directionalLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kDirectionalLight));
	// 方向光源
	lightDesc.spotLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kSpotLight));
	// 点光源
	lightDesc.pointLight->Draw(sCommandList_, static_cast<uint32_t>(Pipeline::InstancedUnitRegister::kPointLight));

	// ドローコール
	sCommandList_->DrawIndexedInstanced(UINT(modelDesc.modelData->indices.size()), instanceNum, 0, 0, 0);
}

void ModelRenderer::LineDraw(ICamera* camera, Line3D* line)
{
	// タイプ設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// パイプライン
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kLine)]);
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

	// バッファ
	sCommandList_->SetGraphicsRootConstantBufferView(0, camera->GetCBuffer()->GetGPUVirtualAddress());

	// 頂点
	sCommandList_->IASetVertexBuffers(0, 1, &line->vbView_);
	// 描画
	sCommandList_->DrawInstanced(UINT(line->GetVertexSize()), 1, 0, 0);

}

void ModelRenderer::TrailDraw(ICamera* camera, MissileTrail* trail)
{
	// タイプ設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// パイプライン
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kMissileTrail)]);
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

	// バッファ
	sCommandList_->SetGraphicsRootConstantBufferView(0, camera->GetCBuffer()->GetGPUVirtualAddress());

	// 頂点
	sCommandList_->IASetVertexBuffers(0, 1, &trail->vbView_);
	// 描画
	sCommandList_->DrawInstanced(UINT(trail->GetVertexSize()), 1, 0, 0);

}

void ModelRenderer::TriangleDraw(ICamera* camera, Triangle3D* triangle)
{
	// タイプ設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// パイプライン
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kTriangle)]);
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

	// バッファ
	sCommandList_->SetGraphicsRootConstantBufferView(0, camera->GetCBuffer()->GetGPUVirtualAddress());

	// 頂点
	sCommandList_->IASetVertexBuffers(0, 1, &triangle->vbView_);
	// 描画
	sCommandList_->DrawInstanced(UINT(triangle->GetVertexSize()), 1, 0, 0);

}
