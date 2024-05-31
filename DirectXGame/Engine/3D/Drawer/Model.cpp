#include "Model.h"
#include "../Loader.h"
#include "../../2D/TextureManager.h"
#include <cassert>

ID3D12GraphicsCommandList* Model::sCommandList_ = nullptr;

void Model::Initialize(const std::string& modelName)
{
	// モデル読み込み
	modelData_ = Loader::LoadObj("Resources", modelName);

	// メッシュ生成
	mesh_ = std::make_unique<Mesh>();
	mesh_->CreateMesh(modelData_.vertices);

	// マテリアル生成
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();

	// テクスチャの設定
	texture_ = modelData_.material.textureHandle;
	//textures_.push_back(TextureManager::Load(modelData_.material.textureFilename));

	// ブレンドモード設定
	blendMode_ = Pipeline::BlendMode::kNormal;
}

Model* Model::CreateObj(const std::string& modelName)
{
	// メモリ確保
	Model* instance = new Model;
	// 初期化
	instance->Initialize(modelName);

	return instance;
}

void Model::Draw(const WorldTransform& worldTransform, ICamera* camera) {

	material_->Update();

	// パイプラインステートの設定
	sCommandList_->SetPipelineState(GraphicsPSO::sModelPipelineStates_[size_t(blendMode_)].Get());

	// ワールド行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kWorldTransform),
		worldTransform.constBuffer_->GetGPUVirtualAddress());
	// ビュープロジェクション行列
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kViewProjection),
		camera->constBuff_->GetGPUVirtualAddress());


	//---メッシュの設定---//
	// 頂点バッファの設定
	sCommandList_->IASetVertexBuffers(0, 1,&mesh_->vbView_);
	// インデックスバッファの設定
	sCommandList_->IASetIndexBuffer(&mesh_->ibView_);

	//---マテリアルの設定---//
	// SRVのセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList_, static_cast<UINT>(ModelRegister::kTexture), modelData_.material.textureHandle);

	// ここでマテリアル設定しろ

	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(ModelRegister::kMaterial), material_->materialBuff_->GetGPUVirtualAddress());

	// ドローコール
	sCommandList_->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);

}

void Model::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// チェック
	assert(Model::sCommandList_ == nullptr);

	// コマンドリストをセット
	sCommandList_ = cmdList;

	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(GraphicsPSO::sModelRootSignature_.Get());

	// プリミティブ形状を設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Model::PostDraw()
{
	// コマンドリストの解除
	sCommandList_ = nullptr;
}
