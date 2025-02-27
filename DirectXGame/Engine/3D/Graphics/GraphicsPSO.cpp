#include "GraphicsPSO.h"
#include "Shader.h"
#include "../../Utility/StringConverter.h"

#include <cassert>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace Pipeline;

std::array<PipelineVariant, size_t(Order::kCountOfParameter)> GraphicsPSO::sPipelines_;


ParticleCSPipeline GraphicsPSO::sParticleGPU_;
GeneralPipeline GraphicsPSO::sSkinningGPU_;

ID3D12Device* GraphicsPSO::sDevice_;

void GraphicsPSO::Initialize(ID3D12Device* device)
{
	// nullチェック
	assert(device);

	sDevice_ = device;

	// スプライト
	CreateSpritePSO();
	// 通常モデル
	CreateModelPSO();
	// パーティクル
	CreateParticlePSO();
	// スキニング
	CreateSkinningModelPSO();
	//CreateSkinningCS();
	// ポストエフェクト
	CreatePostEffectPSO();
	// 箱
	CreateSkyboxPSO();
	// 複数
	CreateInstancedPSO();

	// 線
	CreateLinePSO();
	// 軌跡
	CreateTrailPSO();
	// 三角
	CreateTrianglePSO();
}

void GraphicsPSO::CreateSpritePSO()
{
	BlendPipeline resultPipeline;

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
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

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

	resultPipeline.rootSignature = CreateRootSignature(descriptionRootSignature);
	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ


#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.RenderTarget[0].BlendEnable = false;
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kNone)] = CreatePipelineState(gPipeline);
	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kAlpha)] = CreatePipelineState(gPipeline);

	// 加算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kAdd)] = CreatePipelineState(gPipeline);

	// 減算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kSubtract)] = CreatePipelineState(gPipeline);

	// 乗算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_COLOR);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kMultiply)] = CreatePipelineState(gPipeline);

	// スクリーン合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_INV_DEST_COLOR, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kScreen)] = CreatePipelineState(gPipeline);

#pragma endregion

	sPipelines_[size_t(Pipeline::Order::kSpirte)] = std::move(resultPipeline);

}

void GraphicsPSO::CreateLinePSO()
{
	GeneralPipeline resultPipeline;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"3DLine/3DLine_VS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"3DLine/3DLine_PS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{
		PSOLib::SetInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT)
	},
	{
		PSOLib::SetInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT)
	},
	};
	// グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	gPipeline.VS = Shader::ShaderByteCode(vsBlob.Get());
	gPipeline.PS = Shader::ShaderByteCode(psBlob.Get());

	// サンプルマスク
	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザステート
	D3D12_RASTERIZER_DESC rasterizer = PSOLib::SetRasterizerState(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);

	gPipeline.RasterizerState = rasterizer;
	// デプスステンシルステート
	gPipeline.DepthStencilState.DepthEnable = true;
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1]{};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[1]{};
	// カメラ
	rootparams[0] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	// バイリニアフィルタ
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	// 0~1の範囲r外をリピート
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	// 比較しない

	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;	// ありったけのMipmapを使う
	samplerDesc[0].ShaderRegister = 0;	// レジスタ番号0を使う
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	// 
	resultPipeline.rootSignature = CreateRootSignature(rootSignatureDesc);

	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ

#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineState = CreatePipelineState(gPipeline);

#pragma endregion
	sPipelines_[size_t(Pipeline::Order::kLine)] = std::move(resultPipeline);
}

void GraphicsPSO::CreateTrianglePSO()
{
	GeneralPipeline resultPipeline;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"3DTriangle/3DTriangle_VS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"3DTriangle/3DTriangle_PS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{	// Position
		PSOLib::SetInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT)
	},
	{	// Color
		PSOLib::SetInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT)
	},
	{	// UV
		PSOLib::SetInputLayout("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT)
	},
	};
	// グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	gPipeline.VS = Shader::ShaderByteCode(vsBlob.Get());
	gPipeline.PS = Shader::ShaderByteCode(psBlob.Get());

	// サンプルマスク
	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザステート
	D3D12_RASTERIZER_DESC rasterizer = PSOLib::SetRasterizerState(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);

	gPipeline.RasterizerState = rasterizer;
	// デプスステンシルステート
	gPipeline.DepthStencilState.DepthEnable = true;
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1]{};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[2]{};
	// カメラ
	rootparams[0] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// テクスチャ
	rootparams[1] = PSOLib::InitAsDescriptorTable(1, descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	// バイリニアフィルタ
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	// 0~1の範囲r外をリピート
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	// 比較しない
	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;	// ありったけのMipmapを使う
	samplerDesc[0].ShaderRegister = 0;	// レジスタ番号0を使う
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	resultPipeline.rootSignature = CreateRootSignature(rootSignatureDesc);

	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ

#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineState = CreatePipelineState(gPipeline);

#pragma endregion
	sPipelines_[size_t(Pipeline::Order::kTriangle)] = std::move(resultPipeline);
}

void GraphicsPSO::CreateTrailPSO()
{
	GeneralPipeline resultPipeline;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"Trail/Trail_VS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"Trail/Trail_PS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{	// Position
		PSOLib::SetInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT)
	},
	{	// Color
		PSOLib::SetInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT)
	},
	{	// UV
		PSOLib::SetInputLayout("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT)
	},
	};
	// グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	gPipeline.VS = Shader::ShaderByteCode(vsBlob.Get());
	gPipeline.PS = Shader::ShaderByteCode(psBlob.Get());

	// サンプルマスク
	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザステート
	D3D12_RASTERIZER_DESC rasterizer = PSOLib::SetRasterizerState(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);

	gPipeline.RasterizerState = rasterizer;
	// デプスステンシルステート
	gPipeline.DepthStencilState.DepthEnable = true;
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1]{};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[3]{};
	// テクスチャ
	rootparams[0] = PSOLib::InitAsDescriptorTable(1, descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	// カメラ
	rootparams[1] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// マテリアル
	rootparams[2] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	resultPipeline.rootSignature = CreateRootSignature(rootSignatureDesc);

	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ

#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineState = CreatePipelineState(gPipeline);

#pragma endregion
	sPipelines_[size_t(Pipeline::Order::kTrail)] = std::move(resultPipeline);
}

void GraphicsPSO::CreateModelPSO()
{
	GeneralPipeline resultPipeline;

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
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1]{};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	D3D12_DESCRIPTOR_RANGE descRangeCubeSRV[1]{};
	descRangeCubeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	D3D12_DESCRIPTOR_RANGE descriptorRangeDissolve[1] = {};
	descriptorRangeDissolve[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(ModelRegister::kCountOfParameter)]{};
	//---共通---//
	// テクスチャ
	rootparams[static_cast<int>(ModelRegister::kTexture)] = PSOLib::InitAsDescriptorTable(_countof(descRangeSRV), descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// 環境マップ
	rootparams[static_cast<int>(ModelRegister::kMapTexture)] = PSOLib::InitAsDescriptorTable(_countof(descRangeCubeSRV), descRangeCubeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// ディゾルブ
	rootparams[static_cast<int>(ModelRegister::kDissolveTexture)] = PSOLib::InitAsDescriptorTable(_countof(descriptorRangeDissolve), descriptorRangeDissolve, D3D12_SHADER_VISIBILITY_PIXEL);
	// View
	rootparams[static_cast<int>(ModelRegister::kViewProjection)] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

	//---VS---//
	// WorldTransform
	rootparams[static_cast<int>(ModelRegister::kWorldTransform)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	//---PS---//
	// マテリアル
	rootparams[static_cast<int>(ModelRegister::kMaterial)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	// ライト
	rootparams[static_cast<int>(ModelRegister::kDirectionalLight)] = PSOLib::InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootparams[static_cast<int>(ModelRegister::kPointLight)] = PSOLib::InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootparams[static_cast<int>(ModelRegister::kSpotLight)] = PSOLib::InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_PIXEL);


	// スタティックサンプラー
	//D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	//samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	//samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	// バイリニアフィルタ
	//samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	// 0~1の範囲r外をリピート
	//samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	// 比較しない

	//samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;	// ありったけのMipmapを使う
	//samplerDesc[0].ShaderRegister = 0;	// レジスタ番号0を使う
	//samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	// 
	resultPipeline.rootSignature = CreateRootSignature(rootSignatureDesc);

	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ

#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//blenddesc.RenderTarget[0].BlendEnable = false;

	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineState = CreatePipelineState(gPipeline);

	//// 加算合成
	//blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	//gPipeline.BlendState = blenddesc;
	//// PSO作成
	//result = sDevice_->CreateGraphicsPipelineState(
	//	&gPipeline, IID_PPV_ARGS(
	//		&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kAdd)]));
	//assert(SUCCEEDED(result));
	//// 減算合成
	//blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE);
	//gPipeline.BlendState = blenddesc;
	//// PSO作成
	//result = sDevice_->CreateGraphicsPipelineState(
	//	&gPipeline, IID_PPV_ARGS(
	//		&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kSubtract)]));
	//assert(SUCCEEDED(result));
	//// 乗算合成
	//blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_COLOR);
	//gPipeline.BlendState = blenddesc;
	//// PSO作成
	//result = sDevice_->CreateGraphicsPipelineState(
	//	&gPipeline, IID_PPV_ARGS(
	//		&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kMultiply)]));
	//assert(SUCCEEDED(result));
	//// スクリーン合成
	//blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_INV_DEST_COLOR, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	//gPipeline.BlendState = blenddesc;
	//// PSO作成
	//result = sDevice_->CreateGraphicsPipelineState(
	//	&gPipeline, IID_PPV_ARGS(
	//		&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kScreen)]));
	//assert(SUCCEEDED(result));

#pragma endregion
	sPipelines_[size_t(Pipeline::Order::kModel)] = std::move(resultPipeline);

}

void GraphicsPSO::CreateParticlePSO()
{
	BlendPipeline resultPipeline;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"Particle/ParticleVS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"Particle/ParticlePS.hlsl", L"ps_6_0");
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
	{
		PSOLib::SetInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT)
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
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1]{};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(GPUParticleDrawRegister::kCountOfParameter)]{};
	//---共通---//
	//---VS---//
	// カメラ
	rootparams[static_cast<int>(GPUParticleDrawRegister::kCamera)] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	// 行列
	rootparams[static_cast<int>(GPUParticleDrawRegister::kMatrixs)] = PSOLib::InitAsDescriptorTable(_countof(descRangeSRV), descRangeSRV, D3D12_SHADER_VISIBILITY_VERTEX);
	//---PS---//
	// テクスチャ
	rootparams[static_cast<int>(GPUParticleDrawRegister::kTexture)] = PSOLib::InitAsDescriptorTable(_countof(descRangeSRV), descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// マテリアル
	rootparams[static_cast<int>(GPUParticleDrawRegister::kMaterial)] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_PIXEL);


	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	// 
	resultPipeline.rootSignature = CreateRootSignature(rootSignatureDesc);

	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ

#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kAlpha)] = CreatePipelineState(gPipeline);
	// 加算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kAdd)] = CreatePipelineState(gPipeline);
	// 減算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kSubtract)] = CreatePipelineState(gPipeline);
	// 乗算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_COLOR);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kMultiply)] = CreatePipelineState(gPipeline);
	// スクリーン合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_INV_DEST_COLOR, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kScreen)] = CreatePipelineState(gPipeline);

	sPipelines_[size_t(Pipeline::Order::kParticle)] = std::move(resultPipeline);

	// ComputeShader
	CreateParticleCSPSO();
}

void GraphicsPSO::CreateParticleCSPSO()
{
	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(Pipeline::GPUParticleRegister::kCountOfParameter)]{};
	D3D12_DESCRIPTOR_RANGE descRangeParticle[1]{};
	descRangeParticle[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
	D3D12_DESCRIPTOR_RANGE descRangeListIndex[1]{};
	descRangeListIndex[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
	D3D12_DESCRIPTOR_RANGE descRangeList[1]{};
	descRangeList[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 2);
	//---共通---//
	// 書き込みParticle
	rootparams[static_cast<int>(Pipeline::GPUParticleRegister::kUAVParticle)] = PSOLib::InitAsDescriptorTable(_countof(descRangeParticle), descRangeParticle, D3D12_SHADER_VISIBILITY_ALL);
	// リストの番号とリスト
	rootparams[static_cast<int>(Pipeline::GPUParticleRegister::kUAVFreeListIndex)] = PSOLib::InitAsDescriptorTable(_countof(descRangeListIndex), descRangeListIndex, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[static_cast<int>(Pipeline::GPUParticleRegister::kUAVFreeList)] = PSOLib::InitAsDescriptorTable(_countof(descRangeList), descRangeList, D3D12_SHADER_VISIBILITY_ALL);
	// エミッター
	rootparams[static_cast<int>(Pipeline::GPUParticleRegister::kEmitter)] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// 時間
	rootparams[static_cast<int>(Pipeline::GPUParticleRegister::kPerTime)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	sParticleGPU_.csRootSignature = CreateRootSignature(rootSignatureDesc);

	HRESULT result = S_FALSE;
	ComPtr<IDxcBlob> csBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	computePipelineStateDesc.pRootSignature = sParticleGPU_.csRootSignature.Get();

	// 初期化
	csBlob = Shader::GetInstance()->Compile(L"Particle/InitializeParticleCS.hlsl", L"cs_6_0");
	computePipelineStateDesc.CS = { .pShaderBytecode = csBlob->GetBufferPointer(),.BytecodeLength = csBlob->GetBufferSize() };
	result = sDevice_->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kInitialize)]));
	assert(SUCCEEDED(result));
	// エミット
	csBlob = Shader::GetInstance()->Compile(L"Particle/Emitter/EmitParticleCS.hlsl", L"cs_6_0");
	computePipelineStateDesc.CS = { .pShaderBytecode = csBlob->GetBufferPointer(),.BytecodeLength = csBlob->GetBufferSize() };
	result = sDevice_->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kEmit)]));
	assert(SUCCEEDED(result));
	// 更新
	csBlob = Shader::GetInstance()->Compile(L"Particle/UpdateParticleCS.hlsl", L"cs_6_0");
	computePipelineStateDesc.CS = { .pShaderBytecode = csBlob->GetBufferPointer(),.BytecodeLength = csBlob->GetBufferSize() };
	result = sDevice_->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kUpdate)]));
	assert(SUCCEEDED(result));

}

void GraphicsPSO::CreateCPUParticlePSO()
{
	BlendPipeline resultPipeline;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"Instanced/InstancedModelVS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"Instanced/InstancedModelPS.hlsl", L"ps_6_0");
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
	{
		PSOLib::SetInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT)
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
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1]{};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	D3D12_DESCRIPTOR_RANGE descRangeCubeSRV[1]{};
	descRangeCubeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(CPUParticleRegister::kCountOfParmeter)]{};

}

void GraphicsPSO::CreateSkinningModelPSO()
{
	GeneralPipeline resultPipeline;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"Skinning/Skinning3DModelVS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"Skinning/Skinning3DModelPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	D3D12_INPUT_ELEMENT_DESC weight{};
	weight.SemanticName = "WEIGHT";
	weight.SemanticIndex = 0;
	weight.InputSlot = 1;
	weight.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	weight.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_ELEMENT_DESC index{};
	index.SemanticName = "INDEX";
	index.SemanticIndex = 0;
	index.InputSlot = 1;
	index.Format = DXGI_FORMAT_R32G32B32A32_SINT;
	index.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_ELEMENT_DESC inputLayout[5] = {
	{
		PSOLib::SetInputLayout("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT)
	},
	{
		PSOLib::SetInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT)
	},
	{
		PSOLib::SetInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT)
	},
	{
		weight
	},
	{
		index
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
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1]{};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	D3D12_DESCRIPTOR_RANGE descRangeTextureSRV[1]{};
	descRangeTextureSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	D3D12_DESCRIPTOR_RANGE descRangeCubeSRV[1]{};
	descRangeCubeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(SkinningModelRegister::kCountOfParameter)]{};
	//---共通---//
	// View
	rootparams[static_cast<int>(SkinningModelRegister::kViewProjection)] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// Palette
	rootparams[static_cast<int>(SkinningModelRegister::kMatrixPalette)] = PSOLib::InitAsDescriptorTable(_countof(descRangeSRV), descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//---VS---//
	// WorldTransform
	rootparams[static_cast<int>(SkinningModelRegister::kWorldTransform)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	//---PS---//
	// テクスチャ
	rootparams[static_cast<int>(SkinningModelRegister::kTexture)] = PSOLib::InitAsDescriptorTable(_countof(descRangeTextureSRV), descRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// 環境マップ
	rootparams[static_cast<int>(SkinningModelRegister::kMapTexture)] = PSOLib::InitAsDescriptorTable(_countof(descRangeCubeSRV), descRangeCubeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// マテリアル
	rootparams[static_cast<int>(SkinningModelRegister::kMaterial)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	// ライト
	rootparams[static_cast<int>(SkinningModelRegister::kDirectionalLight)] = PSOLib::InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootparams[static_cast<int>(SkinningModelRegister::kPointLight)] = PSOLib::InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootparams[static_cast<int>(SkinningModelRegister::kSpotLight)] = PSOLib::InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_PIXEL);


	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	// ルートシグネチャ作成
	resultPipeline.rootSignature = CreateRootSignature(rootSignatureDesc);

	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ


#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//blenddesc.RenderTarget[0].BlendEnable = false;
	// αブレンド
	//blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;

	resultPipeline.pipelineState = CreatePipelineState(gPipeline);
#pragma endregion
	sPipelines_[size_t(Pipeline::Order::kSkinningModel)] = std::move(resultPipeline);
}

void GraphicsPSO::CreateSkinningCS()
{
	// CS
	ComPtr<IDxcBlob> csBlob;
	csBlob = Shader::GetInstance()->Compile(L"Skinning/Skinning3DModelCS.hlsl", L"cs_6_0");

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[5]{};
	D3D12_DESCRIPTOR_RANGE descRangePalette[1]{};
	descRangePalette[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	D3D12_DESCRIPTOR_RANGE descRangeInputVertices[1]{};
	descRangeInputVertices[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	D3D12_DESCRIPTOR_RANGE descRangeInfluences[1]{};
	descRangeInfluences[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
	D3D12_DESCRIPTOR_RANGE descRangeSkinnedVertex[1]{};
	descRangeSkinnedVertex[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
	//---共通---//
	// Palette
	rootparams[0] = PSOLib::InitAsDescriptorTable(_countof(descRangePalette), descRangePalette, D3D12_SHADER_VISIBILITY_ALL);
	// InputVertices
	rootparams[1] = PSOLib::InitAsDescriptorTable(_countof(descRangeInputVertices), descRangeInputVertices, D3D12_SHADER_VISIBILITY_ALL);
	// Influences
	rootparams[2] = PSOLib::InitAsDescriptorTable(_countof(descRangeInfluences), descRangeInfluences, D3D12_SHADER_VISIBILITY_ALL);
	// SkinnedVertex
	rootparams[3] = PSOLib::InitAsDescriptorTable(_countof(descRangeSkinnedVertex), descRangeSkinnedVertex, D3D12_SHADER_VISIBILITY_ALL);
	// Info
	rootparams[4] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);


	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	sSkinningGPU_.rootSignature = CreateRootSignature(rootSignatureDesc);

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	computePipelineStateDesc.CS = {
		.pShaderBytecode = csBlob->GetBufferPointer(),
		.BytecodeLength = csBlob->GetBufferSize()
	};
	computePipelineStateDesc.pRootSignature = sSkinningGPU_.rootSignature.Get();
	HRESULT result = S_FALSE;
	result = sDevice_->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&sSkinningGPU_.pipelineState));
}

void GraphicsPSO::CreatePostEffectPSO()
{
	PostEffectPipeline resultPipeline;

#pragma region RootSignature

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRangeの作成
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	D3D12_DESCRIPTOR_RANGE descriptorRangeDissolve[1] = {};
	descriptorRangeDissolve[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	// RootParameter作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
	// PixelShaderのMaterialとVertexShaderのTransform
	D3D12_ROOT_PARAMETER rootParameters[static_cast<int>(PostEffectRegister::kCountOfParameter)] = {};
	// テクスチャ
	rootParameters[static_cast<int>(PostEffectRegister::kTexture)] = PSOLib::InitAsDescriptorTable(_countof(descriptorRange), descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);
	// Dissolveのテクスチャ
	rootParameters[static_cast<int>(PostEffectRegister::kDissolveTexture)] = PSOLib::InitAsDescriptorTable(_countof(descriptorRangeDissolve), descriptorRangeDissolve, D3D12_SHADER_VISIBILITY_PIXEL);

	rootParameters[static_cast<int>(PostEffectRegister::kVignette)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[static_cast<int>(PostEffectRegister::kBlur)] = PSOLib::InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[static_cast<int>(PostEffectRegister::kDissolve)] = PSOLib::InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[static_cast<int>(PostEffectRegister::kNoise)] = PSOLib::InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[static_cast<int>(PostEffectRegister::kHSV)] = PSOLib::InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[static_cast<int>(PostEffectRegister::kBloom)] = PSOLib::InitAsConstantBufferView(6, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[static_cast<int>(PostEffectRegister::kGrayscale)] = PSOLib::InitAsConstantBufferView(7, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	descriptionRootSignature.pParameters = rootParameters;	// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);	// 配列の長さ


	D3D12_STATIC_SAMPLER_DESC staticSampler[1] = {};
	staticSampler[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	// バイリニアフィルタ
	staticSampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	// 0~1の範囲r外をリピート
	staticSampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	// 比較しない

	staticSampler[0].MaxLOD = D3D12_FLOAT32_MAX;	// ありったけのMipmapを使う
	staticSampler[0].ShaderRegister = 0;	// レジスタ番号0を使う
	staticSampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSampler;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSampler);

	// ルートシグネチャ作成
	resultPipeline.rootSignature = CreateRootSignature(descriptionRootSignature);

#pragma endregion
	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	D3D12_INPUT_LAYOUT_DESC inputLayout{ nullptr,0 };

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;

	D3D12_RASTERIZER_DESC rasterrizerDesc{};
	rasterrizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterrizerDesc.FillMode = D3D12_FILL_MODE_SOLID;


	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = resultPipeline.rootSignature.Get();	// RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayout;	// InputLayout
	graphicsPipelineStateDesc.BlendState = blendDesc;	// BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterrizerDesc;	// RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくてよい）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState.DepthEnable = false;
	graphicsPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"PostEffect/FullScreenVS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.VS = { vsBlob->GetBufferPointer(),vsBlob->GetBufferSize() };	// VertexShader

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/FullScreenPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kAlpha)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/GrayscalePS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kGrayScale)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/VignettePS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kVignette)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/GrayscaleVignettePS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kGrayscaleVignette)] = CreatePipelineState(graphicsPipelineStateDesc);


	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/BoxFilterPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kSmoothing)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/GaussianFilterPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kGaussian)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/BloomBlurPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kRadialBlur)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/DissolvePS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kDissolve)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/RandomNoisePS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kNoise)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/LuminanceBasedOutlinePS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kLuminanceOutline)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/BloomPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kBloom)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/BloomVignetteBlurPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kVignetteBlur)] = CreatePipelineState(graphicsPipelineStateDesc);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"PostEffect/GrayscaleBloomPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);
	// シェーダの設定
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),psBlob->GetBufferSize() };	// PixelShader
	// パイプラインステート作成
	resultPipeline.pipelineStates[size_t(PostEffect::kGrayscaleBloom)] = CreatePipelineState(graphicsPipelineStateDesc);
	// 登録
	sPipelines_[size_t(Pipeline::Order::kPostEffect)] = std::move(resultPipeline);
}

void GraphicsPSO::CreateSkyboxPSO()
{
	GeneralPipeline resultPipeline;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"Skybox/SkyboxVS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"Skybox/SkyboxPS.hlsl", L"ps_6_0");
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
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
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
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(Pipeline::SkyBoxRegister::kCountOfParameter)]{};
	//---共通---//
	// テクスチャ
	rootparams[static_cast<int>(Pipeline::SkyBoxRegister::kTexture)] = PSOLib::InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// View
	rootparams[static_cast<int>(Pipeline::SkyBoxRegister::kViewProjection)] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

	//---VS---//
	// WorldTransform
	rootparams[static_cast<int>(Pipeline::SkyBoxRegister::kWorldTransform)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	//---PS---//
	// マテリアル
	rootparams[static_cast<int>(Pipeline::SkyBoxRegister::kMaterial)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	// ライト
	//rootparams[static_cast<int>(ModelRegister::kLight)] = PSOLib::InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_PIXEL);

		
	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	// 
	resultPipeline.rootSignature = CreateRootSignature(rootSignatureDesc);

	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ

#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.RenderTarget[0].BlendEnable = false;
	//gPipeline.BlendState = blenddesc;
	//// PSO作成
	//result = sDevice_->CreateGraphicsPipelineState(
	//	&gPipeline, IID_PPV_ARGS(
	//		&GraphicsPSO::sModelPipelineStates_[size_t(BlendMode::kNone)]));
	//assert(SUCCEEDED(result));

	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineState = CreatePipelineState(gPipeline);
#pragma endregion
	sPipelines_[size_t(Pipeline::Order::kSkybox)] = std::move(resultPipeline);
}

void GraphicsPSO::CreateInstancedPSO()
{
	BlendPipeline resultPipeline;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"Instanced/InstancedModelVS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"Instanced/InstancedModelPS.hlsl", L"ps_6_0");
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
	{
		PSOLib::SetInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT)
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
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 頂点レイアウト
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;


	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1]{};
	descRangeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	D3D12_DESCRIPTOR_RANGE descRangeCubeSRV[1]{};
	descRangeCubeSRV[0] = PSOLib::InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(InstancedUnitRegister::kCountOfParameter)]{};
	//---共通---//
	// テクスチャ
	rootparams[static_cast<int>(InstancedUnitRegister::kTexture)] = PSOLib::InitAsDescriptorTable(_countof(descRangeSRV), descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// 環境マップ
	rootparams[static_cast<int>(InstancedUnitRegister::kMapTexture)] = PSOLib::InitAsDescriptorTable(_countof(descRangeCubeSRV), descRangeCubeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// View
	rootparams[static_cast<int>(InstancedUnitRegister::kViewProjection)] = PSOLib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

	//---VS---//
	// WorldTransform
	rootparams[static_cast<int>(InstancedUnitRegister::kWorldTransform)] = PSOLib::InitAsDescriptorTable(_countof(descRangeSRV), descRangeSRV, D3D12_SHADER_VISIBILITY_VERTEX);

	//---PS---//
	// マテリアル
	rootparams[static_cast<int>(InstancedUnitRegister::kMaterial)] = PSOLib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	// ライト
	rootparams[static_cast<int>(InstancedUnitRegister::kDirectionalLight)] = PSOLib::InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootparams[static_cast<int>(InstancedUnitRegister::kPointLight)] = PSOLib::InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootparams[static_cast<int>(InstancedUnitRegister::kSpotLight)] = PSOLib::InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_PIXEL);


	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0] = PSOLib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	// 
	resultPipeline.rootSignature = CreateRootSignature(rootSignatureDesc);

	gPipeline.pRootSignature = resultPipeline.rootSignature.Get();	// ルートシグネチャ

#pragma region ブレンド
	// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.RenderTarget[0].BlendEnable = false;
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kNone)] = CreatePipelineState(gPipeline);
	// αブレンド
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kAlpha)] = CreatePipelineState(gPipeline);

	// 加算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kAdd)] = CreatePipelineState(gPipeline);

	// 減算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kSubtract)] = CreatePipelineState(gPipeline);

	// 乗算合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_COLOR);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kMultiply)] = CreatePipelineState(gPipeline);

	// スクリーン合成
	blenddesc = PSOLib::SetBlendDesc(D3D12_BLEND_INV_DEST_COLOR, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	gPipeline.BlendState = blenddesc;
	// PSO作成
	resultPipeline.pipelineStates[size_t(BlendMode::kScreen)] = CreatePipelineState(gPipeline);

#pragma endregion

	sPipelines_[size_t(Pipeline::Order::kInstancedModel)] = std::move(resultPipeline);
}

void GraphicsPSO::CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline, Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState)
{
	HRESULT result = S_FALSE;
	// PSO作成
	result = sDevice_->CreateGraphicsPipelineState(&gPipeline, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> GraphicsPSO::CreateRootSignature(D3D12_ROOT_SIGNATURE_DESC desc)
{
	ComPtr<ID3DBlob> errorBlob;
	ComPtr<ID3DBlob> rootSigBlob;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> resultSignature;
	HRESULT result = S_FALSE;
	// シリアライズしてバイナリにする
	result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// ルートシグネチャの生成
	result = sDevice_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&resultSignature));
	assert(SUCCEEDED(result));

	return resultSignature;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> GraphicsPSO::CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc)
{
	// 実際に生成
	Microsoft::WRL::ComPtr<ID3D12PipelineState> resultPipelineState;
	HRESULT result = S_FALSE;
	result = sDevice_->CreateGraphicsPipelineState(&desc,IID_PPV_ARGS(&resultPipelineState));
	assert(SUCCEEDED(result));

	return resultPipelineState;
}

std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, size_t(Pipeline::BlendMode::kCountOfBlendMode)> GraphicsPSO::CreateBlendState()
{
	return std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, size_t(Pipeline::BlendMode::kCountOfBlendMode)>();
}
