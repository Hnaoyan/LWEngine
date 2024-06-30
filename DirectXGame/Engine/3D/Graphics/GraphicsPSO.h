#pragma once
#include "../../Utility/Singleton.h"
#include "../../Base/Utility/DxCreateLib.h"

#include <memory>
#include <array>
#include <wrl.h>
#include <variant>

#include <d3d12.h>
#include <dxcapi.h>

namespace Pipeline
{
	enum class Order : int {
		kSkybox,
		kSpirte,
		kModel,
		kSkinningModel,
		kParticle,
		kPostEffect,
		kInstancedModel,
		kCountOfParameter,
	};
	// 通常モデルのレジスタ番号
	enum class ModelRegister : int {
		kMaterial,
		kTexture,
		kMapTexture,
		kWorldTransform,
		kViewProjection,
		kDirectionalLight,
		kPointLight,
		kSpotLight,
		// サイズを取得する為の値
		kCountOfParameter,
	};

	enum class InstancedUnitRegister : int {
		kMaterial,
		kTexture,
		kMapTexture,
		kWorldTransform,
		kViewProjection,
		kDirectionalLight,
		kPointLight,
		kSpotLight,
		// サイズを取得する為の値
		kCountOfParameter,
	};

	// スキニングモデル用のレジスタ番号
	enum class SkinningModelRegister : int {
		kMaterial,
		kTexture,
		kMapTexture,
		kWorldTransform,
		kViewProjection,
		kDirectionalLight,
		kPointLight,
		kSpotLight,

		kMatrixPalette,
		kCountOfParameter,
	};

	// パーティクルのレジスタ用
	enum class ParticleRegister : int {
		kMaterial,
		kTexture,
		kWorldTransform,
		kViewProjection,

		kCountOfParameter,
	};

	enum class BlendMode : int {
		kNone,
		kNormal,
		kAdd,
		kSubtract,
		kMultiply,
		kScreen,
		// サイズカウント
		kCountOfBlendMode,
	};

	enum class DrawType : int {
		kModel,
		kParticle,

		// サイズカウント
		kCountOfDrawer,
	};

	enum class PostEffectType : int {
		kNormal,
		kGrayScale,
		kVignette,
		kSmoothing,
		kGaussian,	
		kCountOfType,
	};

}

struct GeneralPipeline
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
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
	// 統合
	static std::array<PipelineVariant, size_t(Order::kCountOfParameter)>sPipelines_;

	// Particle用（インスタンシング
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sParticlePipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sParticleRootSignature_;

	// Skybox用
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sSkyboxPipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sSkyboxRootSignature_;

private:
	/// <summary>
	/// 2DSprite作成
	/// </summary>
	static void CreateSpritePSO();
	/// <summary>
	/// 3DModel作成
	/// </summary>
	static void CreateModelPSO();
	/// <summary>
	/// パーティクル作成
	/// </summary>
	static void CreateParticlePSO();
	/// <summary>
	/// Skinning作成
	/// </summary>
	static void CreateSkinningModelPSO();
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

	/// <summary>
	/// PSO作成
	/// </summary>
	/// <param name="gPipeline"></param>
	/// <param name="pipelineState"></param>
	static void CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline, Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState);

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature(D3D12_ROOT_SIGNATURE_DESC desc);

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);

private:
	static ID3D12Device* sDevice_;
};

