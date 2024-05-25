#include "Model.h"
#include "../Loader.h"
#include <cassert>

ID3D12GraphicsCommandList* Model::sCommandList_ = nullptr;

void Model::Initialize(const std::string& modelName)
{
	// モデル読み込み
	ModelData data = Loader::LoadObj("Resoureces", modelName);

	mesh_ = std::make_unique<Mesh>();
	mesh_->CreateMesh(data.vertices);

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
