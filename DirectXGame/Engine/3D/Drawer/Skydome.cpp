#include "Skydome.h"
#include "../../Base/DirectXCommon.h"
#include "../../2D/TextureManager.h"
#include "../../3D/CBuffer.h"
#include "../../Base/Utility/DxCreateLib.h"
#include "Model.h"

Skydome* Skydome::CreateSkydome()
{
    // メモリ確保
    Skydome* instance = new Skydome;
    // 初期化
    instance->Initialize();

    return instance;
}

void Skydome::Initialize()
{
	// 分割数	
	subdivision_ = 512;
	// 頂点数
	vertexIndex_ = subdivision_ * subdivision_ * 6;

	// 頂点作成
	CreateVertex();
	// マテリアル作成
	CreateMaterial();

	textureHandle_ = TextureManager::Load("Resources/default/uvChecker.png");

}

void Skydome::CreateVertex()
{
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点
	{
		// 頂点バッファの生成
		vertBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(VertexData) * vertexIndex_);

		// ビュー作成
		vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
		vbView_.SizeInBytes = UINT(sizeof(VertexData) * vertexIndex_);
		vbView_.StrideInBytes = sizeof(VertexData);

		// データ書き込み
		vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertData_));
	}

	///---球体の頂点作成
	
	// 経度分割１つ分の角度 φd
	const float kLonEvery = float(3.14f) * 2.0f / float(subdivision_);
	// 緯度分割１つ分の角度 θd
	const float kLatEvery = float(3.14f) / float(subdivision_);
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < subdivision_; ++latIndex) {
		float lat = float(-3.14f) / 2.0f + kLatEvery * latIndex; // θ
		for (uint32_t lonIndex = 0; lonIndex < subdivision_; ++lonIndex) {
			uint32_t startIndex = (latIndex * subdivision_ + lonIndex) * 6;
			float lon = lonIndex * kLonEvery; // φ
			// 頂点にデータを入力する。基準点a 1
			vertData_[startIndex].position.x = std::cosf(lat) * std::cosf(lon);
			vertData_[startIndex].position.y = std::sinf(lat);
			vertData_[startIndex].position.z = std::cosf(lat) * std::sinf(lon);
			vertData_[startIndex].position.w = 1.0f;
			vertData_[startIndex].texcoord.x = float(lonIndex) / float(subdivision_);
			vertData_[startIndex].texcoord.y = 1.0f - float(latIndex) / float(subdivision_);

			vertData_[startIndex].normal.x = vertData_[startIndex].position.x;
			vertData_[startIndex].normal.y = vertData_[startIndex].position.y;
			vertData_[startIndex].normal.z = vertData_[startIndex].position.z;
			// 頂点b 2
			vertData_[startIndex + 1].position.x = std::cosf(lat + kLatEvery) * std::cosf(lon);
			vertData_[startIndex + 1].position.y = std::sinf(lat + kLatEvery);
			vertData_[startIndex + 1].position.z = std::cosf(lat + kLatEvery) * std::sinf(lon);
			vertData_[startIndex + 1].position.w = 1.0f;
			vertData_[startIndex + 1].texcoord.x = float(lonIndex) / float(subdivision_);
			vertData_[startIndex + 1].texcoord.y = 1.0f - float(latIndex) / float(subdivision_);

			vertData_[startIndex + 1].normal.x = vertData_[startIndex + 1].position.x;
			vertData_[startIndex + 1].normal.y = vertData_[startIndex + 1].position.y;
			vertData_[startIndex + 1].normal.z = vertData_[startIndex + 1].position.z;
			// 頂点c 3
			vertData_[startIndex + 2].position.x = std::cosf(lat) * std::cosf(lon + kLonEvery);
			vertData_[startIndex + 2].position.y = std::sinf(lat);
			vertData_[startIndex + 2].position.z = std::cosf(lat) * std::sinf(lon + kLonEvery);
			vertData_[startIndex + 2].position.w = 1.0f;
			vertData_[startIndex + 2].texcoord.x = float(lonIndex) / float(subdivision_);
			vertData_[startIndex + 2].texcoord.y = 1.0f - float(latIndex) / float(subdivision_);

			vertData_[startIndex + 2].normal.x = vertData_[startIndex + 2].position.x;
			vertData_[startIndex + 2].normal.y = vertData_[startIndex + 2].position.y;
			vertData_[startIndex + 2].normal.z = vertData_[startIndex + 2].position.z;
			// 頂点c 4
			vertData_[startIndex + 3].position.x = std::cosf(lat) * std::cosf(lon + kLonEvery);
			vertData_[startIndex + 3].position.y = std::sinf(lat);
			vertData_[startIndex + 3].position.z = std::cosf(lat) * std::sinf(lon + kLonEvery);
			vertData_[startIndex + 3].position.w = 1.0f;
			vertData_[startIndex + 3].texcoord.x = float(lonIndex) / float(subdivision_);
			vertData_[startIndex + 3].texcoord.y = 1.0f - float(latIndex) / float(subdivision_);

			vertData_[startIndex + 3].normal.x = vertData_[startIndex + 3].position.x;
			vertData_[startIndex + 3].normal.y = vertData_[startIndex + 3].position.y;
			vertData_[startIndex + 3].normal.z = vertData_[startIndex + 3].position.z;
			// 頂点b 5
			vertData_[startIndex + 4].position.x = std::cosf(lat + kLatEvery) * std::cosf(lon);
			vertData_[startIndex + 4].position.y = std::sinf(lat + kLatEvery);
			vertData_[startIndex + 4].position.z = std::cosf(lat + kLatEvery) * std::sinf(lon);
			vertData_[startIndex + 4].position.w = 1.0f;
			vertData_[startIndex + 4].texcoord.x = float(lonIndex) / float(subdivision_);
			vertData_[startIndex + 4].texcoord.y = 1.0f - float(latIndex) / float(subdivision_);

			vertData_[startIndex + 4].normal.x = vertData_[startIndex + 4].position.x;
			vertData_[startIndex + 4].normal.y = vertData_[startIndex + 4].position.y;
			vertData_[startIndex + 4].normal.z = vertData_[startIndex + 4].position.z;
			// 頂点d 6
			vertData_[startIndex + 5].position.x = std::cosf(lat + kLatEvery) * std::cosf(lon + kLonEvery);
			vertData_[startIndex + 5].position.y = std::sinf(lat + kLatEvery);
			vertData_[startIndex + 5].position.z = std::cosf(lat + kLatEvery) * std::sinf(lon + kLonEvery);
			vertData_[startIndex + 5].position.w = 1.0f;
			vertData_[startIndex + 5].texcoord.x = float(lonIndex) / float(subdivision_);
			vertData_[startIndex + 5].texcoord.y = 1.0f - float(latIndex) / float(subdivision_);

			vertData_[startIndex + 5].normal.x = vertData_[startIndex + 5].position.x;
			vertData_[startIndex + 5].normal.y = vertData_[startIndex + 5].position.y;
			vertData_[startIndex + 5].normal.z = vertData_[startIndex + 5].position.z;
		}
	}
}

void Skydome::CreateMaterial()
{
	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	// 無し
	enableLighting_ = false;

	shininess_ = 100.0f;

	// リソース作成
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// マテリアルバッファの生成
	materialBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(CBufferDataMaterial));

	materialBuff_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	// データの転送
	materialData_->color = color_;
	materialData_->enableLighting = enableLighting_;
	materialData_->uvTransform = Matrix4x4::MakeUvTransformMatirx(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
	materialData_->shininess = shininess_;
}

void Skydome::Draw(const ModelDrawDesc& desc)
{
	GeneralPipeline pipeline = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kModel)]);
	// ルートシグネチャの設定
	Model::sCommandList_->SetGraphicsRootSignature(pipeline.rootSignature.Get());
	// パイプラインステートの設定
	Model::sCommandList_->SetPipelineState(pipeline.pipelineState.Get());

	// ワールド行列
	Model::sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::ModelRegister::kWorldTransform),
		desc.worldTransform->GetCBuffer()->GetGPUVirtualAddress());
	// ビュープロジェクション行列
	Model::sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::ModelRegister::kViewProjection),
		desc.camera->GetCBuffer()->GetGPUVirtualAddress());

	//---メッシュの設定---//
	// 頂点バッファの設定
	Model::sCommandList_->IASetVertexBuffers(0, 1, &vbView_);

	//---マテリアルの設定---//
	// SRVのセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		Model::sCommandList_, static_cast<UINT>(Pipeline::ModelRegister::kTexture), textureHandle_);
	// マテリアル
	Model::sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(Pipeline::ModelRegister::kMaterial), materialBuff_->GetGPUVirtualAddress());

	// ライト
	desc.directionalLight->Draw(Model::sCommandList_, static_cast<uint32_t>(Pipeline::ModelRegister::kDirectionalLight));

	// ドローコール
	Model::sCommandList_->DrawInstanced(UINT(this->vertexIndex_), 1, 0, 0);

}
