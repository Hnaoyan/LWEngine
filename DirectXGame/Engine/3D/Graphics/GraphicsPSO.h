#pragma once
#include "../../Utility/Singleton.h"
#include "../../Base/Utility/DxCreateLib.h"

#include <memory>
#include <array>
#include <wrl.h>

#include <d3d12.h>
#include <dxcapi.h>

namespace Pipeline
{
	enum class ModelRegister : int {
		kMaterial,
		kTexture,
		kWorldTransform,
		kViewProjection,
		//kLight,

		// サイズを取得する為の値
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

}

class GraphicsPSO : public Singleton<GraphicsPSO>
{
private:
	// パイプライン用の関数をまとめたライブラリのパス
	using PSOLib = DxCreateLib::PSOLib;

public:

	static void Initialize(ID3D12Device* device);

public:
	// Sprite用
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::BlendMode::kCountOfBlendMode)> sSpritePipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sSpriteRootSignature_;

	// Model用
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::BlendMode::kCountOfBlendMode)> sModelPipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sModelRootSignature_;

	// Particle用（インスタンシング
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sParticlePipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sParticleRootSignature_;

private:

	static void CreateSpritePSO();
	static void CreateModelPSO();
	static void CreateParticlePSO();


	/// <summary>
	/// RootSignature作成関数（まだ
	/// </summary>
	/// <param name="rootParameters"></param>
	/// <param name="rootParamSize"></param>
	/// <param name="staticSamplers"></param>
	/// <param name="samplerSize"></param>
	static void CreateRootSignature(D3D12_ROOT_PARAMETER* rootParameters, size_t rootParamSize, D3D12_STATIC_SAMPLER_DESC* staticSamplers, size_t samplerSize, Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature);

	/// <summary>
	/// PSO作成
	/// </summary>
	/// <param name="gPipeline"></param>
	/// <param name="pipelineState"></param>
	static void CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline, Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState);

private:
	static ID3D12Device* sDevice_;

private:


};

