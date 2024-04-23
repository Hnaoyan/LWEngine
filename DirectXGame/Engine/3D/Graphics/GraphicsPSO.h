#pragma once
#include "../../Utility/Singleton.h"

#include <memory>
#include <array>
#include <wrl.h>
#include <d3d12.h>

namespace Pipeline
{
	enum class RootParameter : int {
		kMaterial,
		kTexture,
		kWorldTransform,
		kViewProjection,
		kLight,

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
public:

	static void CreatePipeline();

public:
	// Model用
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::BlendMode::kCountOfBlendMode)> sPipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;

	// Particle用（インスタンシング
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sParticlePipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sParticleRootSignature_;

private:

	void CreateModelPSO();
	void CreateParticlePSO();

};

