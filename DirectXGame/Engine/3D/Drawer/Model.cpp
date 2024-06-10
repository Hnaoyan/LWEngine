#include "Model.h"
#include "../Loader.h"
#include "../../2D/TextureManager.h"
#include <cassert>
#include "../../Animation/Animation.h"

ID3D12GraphicsCommandList* Model::sCommandList_ = nullptr;
std::string Model::sDirectoryPath = "Resources";

void Model::Initialize(const std::string& modelName, LoadExtension ex)
{
	// モデル読み込み
	modelData_ = Loader::LoadAssimp(sDirectoryPath + "/" + modelName, modelName);
	ex;
	// メッシュ生成
	mesh_ = std::make_unique<Mesh>();
	mesh_->CreateMeshObj(&modelData_);

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
	// マテリアル更新
	material_->Update();

	// Descの設定・更新
	//Matrix4x4 local = Matrix4x4::Multiply(desc.localMatrix, modelData_.rootNode.localMatrix);
	//desc.worldTransform->SetModelData(local);
	//desc.worldTransform->UpdateMatrix();

	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(GraphicsPSO::sModelRootSignature_.Get());
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(GraphicsPSO::sModelPipelineStates_[size_t(blendMode_)].Get());

	// ワールド行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kWorldTransform),
		desc.worldTransform->constBuffer_->GetGPUVirtualAddress());
	// ビュープロジェクション行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kViewProjection),
		desc.camera->constBuff_->GetGPUVirtualAddress());

	//---メッシュの設定---//
	// 頂点バッファの設定
	sCommandList_->IASetVertexBuffers(0, 1,&mesh_->vbView_);
	// インデックスバッファの設定
	//sCommandList_->IASetIndexBuffer(&mesh_->ibView_);

	//---マテリアルの設定---//
	// SRVのセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(ModelRegister::kTexture), modelData_.material.textureHandle);
	// マテリアル
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kMaterial), material_->materialBuff_->GetGPUVirtualAddress());

	// ドローコール
	sCommandList_->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);

}

void Model::SkinningDraw(const ModelDrawDesc& desc, Animation* animation)
{
	// マテリアル更新
	material_->Update();

	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(GraphicsPSO::sSkinningModelRootSignature_.Get());
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(GraphicsPSO::sSkinningModelPipelineStates_[size_t(blendMode_)].Get());

	// ワールド行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(SkinningModelRegister::kWorldTransform),
		desc.worldTransform->constBuffer_->GetGPUVirtualAddress());
	// ビュープロジェクション行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(SkinningModelRegister::kViewProjection),
		desc.camera->constBuff_->GetGPUVirtualAddress());

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
		sCommandList_, static_cast<UINT>(SkinningModelRegister::kTexture), modelData_.material.textureHandle);

	// Skinning
	sCommandList_->SetGraphicsRootDescriptorTable(
		static_cast<UINT>(SkinningModelRegister::kMatrixPalette), animation->skinCluster_.paletteSrvHandle.second);

	// マテリアル
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(SkinningModelRegister::kMaterial), material_->materialBuff_->GetGPUVirtualAddress());

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
