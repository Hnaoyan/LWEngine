#pragma once
#include "../../Utility/Singleton.h"
#include "../../Base/Utility/DxCreateLib.h"
#include "PipelineStructs.h"

#include <memory>
#include <array>
#include <wrl.h>
#include <variant>

#include <d3d12.h>
#include <dxcapi.h>
struct GeneralPipeline
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

struct ParticleCSPipeline 
{
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::GPUParticlePipeline::kCountOfProcess)> pipelineStates;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> csRootSignature;
};

struct CSPipeline 
{
	GeneralPipeline graphicsPipeline;
	ParticleCSPipeline computeShaderPipeline;
};

struct BlendPipeline
{
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::BlendMode::kCountOfBlendMode)> pipelineStates;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

struct PostEffectPipeline
{
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::PostEffectType::kCountOfType)> pipelineStates;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

using PipelineVariant = std::variant<GeneralPipeline, BlendPipeline, PostEffectPipeline>;

class GraphicsPSO : public Singleton<GraphicsPSO>
{
private:
	// パイプライン用の関数をまとめたライブラリのパス
	using PSOLib = DxCreateLib::PSOLib;
	using BlendMode = Pipeline::BlendMode;
	using PostEffect = Pipeline::PostEffectType;
	using Order = Pipeline::Order;

public:

	static void Initialize(ID3D12Device* device);

public:
	// 描画関係のパイプライン配列
	static std::array<PipelineVariant, size_t(Order::kCountOfParameter)>sPipelines_;

	// GPU用のPipeline
	static ParticleCSPipeline sParticleGPU_;
	static GeneralPipeline sSkinningGPU_;

private:
#pragma region PSO作成関数
	/// <summary>
	/// 2DSprite作成
	/// </summary>
	static void CreateSpritePSO();

	// 線
	static void CreateLinePSO();
	// 三角
	static void CreateTrianglePSO();

	static void CreateTrailPSO();

	/// <summary>
	/// 3DModel作成
	/// </summary>
	static void CreateModelPSO();
	/// <summary>
	/// パーティクル作成
	/// </summary>
	static void CreateParticlePSO();
	static void CreateParticleCSPSO();
	/// <summary>
	/// Skinning作成
	/// </summary>
	static void CreateSkinningModelPSO();
	static void CreateSkinningCS();
	/// <summary>
	/// ポストエフェクト作成
	/// </summary>
	static void CreatePostEffectPSO();
	/// <summary>
	/// Skybox作成
	/// </summary>
	static void CreateSkyboxPSO();
	/// <summary>
	/// インスタンシング用
	/// </summary>
	static void CreateInstancedPSO();
#pragma endregion


	/// <summary>
	/// PSO作成
	/// </summary>
	/// <param name="gPipeline"></param>
	/// <param name="pipelineState"></param>
	static void CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline, Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState);

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature(D3D12_ROOT_SIGNATURE_DESC desc);

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);

	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::BlendMode::kCountOfBlendMode)> CreateBlendState();

	static D3D12_GRAPHICS_PIPELINE_STATE_DESC CreatePipelineDesc();

private:
	static ID3D12Device* sDevice_;
};

