#include "GraphicsPSO.h"
#include "Shader.h"
#include "../../Utility/StringConverter.h"

#include <cassert>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace Pipeline;

std::array<ComPtr<ID3D12PipelineState>,
	size_t(BlendMode::kCountOfBlendMode)> GraphicsPSO::sSpritePipelineStates_;
ComPtr<ID3D12RootSignature> GraphicsPSO::sSpriteRootSignature_;

// Model用
std::array<ComPtr<ID3D12PipelineState>,
	size_t(BlendMode::kCountOfBlendMode)> GraphicsPSO::sModelPipelineStates_;
ComPtr<ID3D12RootSignature> GraphicsPSO::sModelRootSignature_;

// Particle用（インスタンシング
ComPtr<ID3D12PipelineState> GraphicsPSO::sParticlePipelineStates_;
ComPtr<ID3D12RootSignature> GraphicsPSO::sParticleRootSignature_;
ID3D12Device* GraphicsPSO::sDevice_;

void GraphicsPSO::Initialize(ID3D12Device* device)
{
	// nullチェック
	assert(device);

	sDevice_ = device;

	// Sprite
	CreateSpritePSO();
	// Model
	CreateModelPSO();
	// Particle
	CreateParticlePSO();

}

void GraphicsPSO::CreateSpritePSO()
{
	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"Sprite_VS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"Sprite_PS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	// xy
			PSOLib::SetInputLayout("POSITION",DXGI_FORMAT_R32G32B32_FLOAT)
		},
		{	// uv
			PSOLib::SetInputLayout("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT)
		},
		//{	// uv
		//	PSOLib::SetInputLayout("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT)
		//},
	};

	// パイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	gPipeline.VS = Shader::ShaderByteCode(vsBlob.Get());		// VertexShader
	gPipeline.PS = Shader::ShaderByteCode(psBlob.Get());		// PixelShader

	// サンプルマスクの設定
	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ラスタライザステートの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc = PSOLib::SetRasterizerState(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	rasterizerDesc.DepthClipEnable = true;

	// デプスステンシルステートの設定
	D3D12_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	dsDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	//---パイプラインに設定---//
	// ラスタライザの設定
	gPipeline.RasterizerState = rasterizerDesc;
	// デプスステンシルの設定
	gPipeline.DepthStencilState = dsDesc;

	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);

	// 利用するトポロジ（形状）のタイプ。三角形
	gPipeline.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// 書き込むRTVの情報
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;

	// デスクリプタレンジの作成
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1] = {};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t0レジスタ

	// ルートパラメータの作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	// 行列など
	rootParameters[0] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// テクスチャ
	rootParameters[1] = PSOLib::InitAsDescriptorTable(1, descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	/// スタティックサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	//staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	// 0~1の範囲外をリピート
	//staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	descriptionRootSignature.pParameters = rootParameters;	// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> errorBlob;
	ComPtr<ID3DBlob> rootSigBlob;

	// シリアライズしてバイナリにする
	result = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// ルートシグネチャの生成
	result = sDevice_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&sSpriteRootSignature_));
	assert(SUCCEEDED(result));

	gPipeline.pRootSignature = sSpriteRootSignature_.Get();	// ルートシグネチャ

	//CreateRootSignature(rootParameters, static_cast<size_t>(rootParameters),staticSamplers);

#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.RenderTarget[0].BlendEnable = false;
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sSpritePipelineStates_[size_t(BlendMode::kNone)]));
	assert(SUCCEEDED(result));

	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sSpritePipelineStates_[size_t(BlendMode::kNormal)]));
	assert(SUCCEEDED(result));

	// 加算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sSpritePipelineStates_[size_t(BlendMode::kAdd)]));
	assert(SUCCEEDED(result));

	// 減算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sSpritePipelineStates_[size_t(BlendMode::kSubtract)]));
	assert(SUCCEEDED(result));

	// 乗算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_COLOR);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sSpritePipelineStates_[size_t(BlendMode::kMultiply)]));
	assert(SUCCEEDED(result));

	// スクリーン合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_INV_DEST_COLOR, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sSpritePipelineStates_[size_t(BlendMode::kScreen)]));
	assert(SUCCEEDED(result));

#pragma endregion

}

void GraphicsPSO::CreateModelPSO()
{
	HRESULT result = S_FALSE;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"3DModelVS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"3DModelPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{
		PSOLib::SetInputLayout("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT)
	},
	{
		PSOLib::SetInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT)
	},
	{
		PSOLib::SetInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT)
	},
	};
	// グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	gPipeline.VS = Shader::ShaderByteCode(vsBlob.Get());
	gPipeline.PS = Shader::ShaderByteCode(psBlob.Get());

	// サンプルマスク
	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザステート
	D3D12_RASTERIZER_DESC rasterizer = PSOLib::SetRasterizerState(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK);
	gPipeline.RasterizerState = rasterizer;
	// デプスステンシルステート
	gPipeline.DepthStencilState.DepthEnable = true;
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(ModelRegister::kCountOfParameter)]{};
	// テクスチャ
	rootparams[static_cast<int>(ModelRegister::kTexture)] = PSOLib::InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// WorldTransform
	rootparams[static_cast<int>(ModelRegister::kWorldTransform)] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	// View
	rootparams[static_cast<int>(ModelRegister::kViewProjection)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	// マテリアル
	rootparams[static_cast<int>(ModelRegister::kMaterial)] = PSOLib::InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	// ライト
	//rootparams[static_cast<int>(ModelRegister::kLight)] = PSOLib::InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_PIXEL);


	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	//samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	ComPtr<ID3DBlob> errorBlob;
	ComPtr<ID3DBlob> rootSigBlob;

	// シリアライズしてバイナリにする
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// ルートシグネチャの生成
	result = sDevice_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&sModelRootSignature_));
	assert(SUCCEEDED(result));

	gPipeline.pRootSignature = sModelRootSignature_.Get();	// ルートシグネチャ

	#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.RenderTarget[0].BlendEnable = false;
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kNone)]));
	assert(SUCCEEDED(result));

	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kNormal)]));
	assert(SUCCEEDED(result));

	// 加算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kAdd)]));
	assert(SUCCEEDED(result));

	// 減算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kSubtract)]));
	assert(SUCCEEDED(result));

	// 乗算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_COLOR);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kMultiply)]));
	assert(SUCCEEDED(result));

	// スクリーン合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_INV_DEST_COLOR, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(
		&gPipeline, IID_PPV_ARGS(
			&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kScreen)]));
	assert(SUCCEEDED(result));

#pragma endregion

}

void GraphicsPSO::CreateParticlePSO()
{
}

void GraphicsPSO::CreateRootSignature(D3D12_ROOT_PARAMETER rootParameters[], size_t rootParamSize, D3D12_STATIC_SAMPLER_DESC* staticSamplers, size_t samplerSize, Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature)
{
	rootParameters, rootParamSize, staticSamplers, samplerSize, rootSignature;
	//HRESULT result = S_FALSE;
	//ComPtr<ID3DBlob> errorBlob;
	//ComPtr<ID3DBlob> rootSigBlob;

	//D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	//descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//
	//descriptionRootSignature.pParameters = rootParameters;	// ルートパラメータ配列へのポインタ
	//descriptionRootSignature.NumParameters = _countof(rootParameters);

	//descriptionRootSignature.pStaticSamplers = staticSamplers;
	//descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	//result = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &rootSigBlob, &errorBlob);
	//if (FAILED(result)) {
	//	Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	//	assert(false);
	//}
	//// ルートシグネチャの生成
	//result = sDevice_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
	//	rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	//assert(SUCCEEDED(result));

}

void GraphicsPSO::CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline, Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState)
{
	HRESULT result = S_FALSE;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(&gPipeline, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}
