#include "Model.h"
#include "../Loader.h"
#include "../../2D/TextureManager.h"
#include <cassert>
#include "../../Animation/Animation.h"

ID3D12GraphicsCommandList* Model::sCommandList_ = nullptr;
std::string Model::sDirectoryPath = "Resources";
GeneralPipeline Model::sPipeline_;

void Model::Initialize(const std::string& modelName, LoadExtension ex)
{
	// モデル読み込み
	modelData_ = Loader::LoadAssimp(sDirectoryPath + "/" + modelName, modelName);
	ex;
	// メッシュ生成
	mesh_ = std::make_unique<Mesh>();
	mesh_->CreateMesh(&modelData_);

	// マテリアル生成
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();

	// テクスチャの設定
	texture_ = modelData_.material.textureHandle;

	// ブレンドモード設定
	blendMode_ = Pipeline::BlendMode::kNormal;
}

void Model::Initialize(const std::string& modelName)
{
	// モデル読み込み
	modelData_ = Loader::LoadGlTF(sDirectoryPath + "/" + modelName, modelName);
	//modelData_ = Loader::LoadGlTF(sDirectoryPath + "/" + modelName, modelName);
	// アニメーションの読み込み
	modelData_.animData = Loader::LoadAnimationFile(sDirectoryPath + "/" + modelName, modelName);

	// メッシュ生成
	mesh_ = std::make_unique<Mesh>();
	mesh_->CreateMesh(&modelData_);

	// マテリアル生成
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();

	// テクスチャの設定
	texture_ = modelData_.material.textureHandle;

	// ブレンドモード設定
	blendMode_ = Pipeline::BlendMode::kNormal;
}

Model* Model::CreateObj(const std::string& modelName, LoadExtension ex)
{
	// メモリ確保
	Model* instance = new Model;
	// 初期化
	ex;
	instance->Initialize(modelName);

	return instance;
}

Model* Model::CreateDefault(const std::string& modelName)
{
	// メモリ確保
	Model* instance = new Model;
	// 初期化
	LoadExtension ex = LoadExtension::kObj;
	instance->Initialize(modelName, ex);

	return instance;
}

void Model::Draw(const ModelDrawDesc& desc) {
	// パイプラインの設定
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kModel)]);

	// マテリアル更新
	material_->Update();

	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

	// ワールド行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kWorldTransform),
		desc.worldTransform->GetCBuffer()->GetGPUVirtualAddress());
	// ビュープロジェクション行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kViewProjection),
		desc.camera->GetCBuffer()->GetGPUVirtualAddress());

	//---メッシュの設定---//
	// 頂点バッファの設定
	sCommandList_->IASetVertexBuffers(0, 1,&mesh_->vbView_);
	// インデックスバッファの設定
	sCommandList_->IASetIndexBuffer(&mesh_->ibView_);

	//---マテリアルの設定---//
	// SRVのセット
	if (desc.texture != 100) {
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
			sCommandList_, static_cast<UINT>(ModelRegister::kTexture), desc.texture);
	}
	else {
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
			sCommandList_, static_cast<UINT>(ModelRegister::kTexture), modelData_.material.textureHandle);
	}
	// 環境マップ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(ModelRegister::kMapTexture), TextureManager::sEnvironmentTexture);
	// マテリアル
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kMaterial), material_->materialBuff_->GetGPUVirtualAddress());


	// ライト
	if (desc.directionalLight) {
		desc.directionalLight->Draw(sCommandList_, static_cast<uint32_t>(ModelRegister::kDirectionalLight));
	}
	if (desc.spotLight) {
		desc.spotLight->Draw(sCommandList_, static_cast<uint32_t>(ModelRegister::kSpotLight));
	}
	if (desc.pointLight) {
		desc.pointLight->Draw(sCommandList_, static_cast<uint32_t>(ModelRegister::kPointLight));
	}

	// ドローコール
	sCommandList_->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}

void Model::SkinningDraw(const ModelDrawDesc& desc, Animation* animation,uint32_t texture)
{
	// パイプラインの設定
	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kSkinningModel)]);
	// マテリアル更新
	material_->Update();

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
		mesh_->vbView_,
		animation->skinCluster_.influenceBufferView
	};

	sCommandList_->IASetVertexBuffers(0, 2, vbvs);
	// インデックスバッファの設定
	sCommandList_->IASetIndexBuffer(&mesh_->ibView_);

	//---マテリアルの設定---//
	// SRVのセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(SkinningModelRegister::kTexture), texture);

	// 環境マップ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(SkinningModelRegister::kMapTexture), TextureManager::sEnvironmentTexture);
	// Skinning
	sCommandList_->SetGraphicsRootDescriptorTable(
		static_cast<UINT>(SkinningModelRegister::kMatrixPalette), animation->skinCluster_.paletteSrvHandle.second);

	// マテリアル
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(SkinningModelRegister::kMaterial), material_->materialBuff_->GetGPUVirtualAddress());

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
	sCommandList_->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}

void Model::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// チェック
	assert(Model::sCommandList_ == nullptr);

	// コマンドリストをセット
	sCommandList_ = cmdList;

	// プリミティブ形状を設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Model::PostDraw()
{
	// コマンドリストの解除
	sCommandList_ = nullptr;
}
