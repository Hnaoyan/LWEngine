#pragma once
#include <d3d12.h>
#include "../Animation/Animation.h"
#include "ModelData.h"
#include "Graphics/GraphicsPSO.h"

class ModelRenderer
{
private: 
	// 省略パス
	using ModelRegister = Pipeline::ModelRegister;
	using SkinningModelRegister = Pipeline::SkinningModelRegister;
	using BlendMode = Pipeline::BlendMode;

public:
	// public static
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
private:
	// private static
	// パイプライン
	static GeneralPipeline sPipeline_;

public: // 準備用の関数
	/// <summary>
	/// 描画前
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画後
	/// </summary>
	static void PostDraw();

public:
	/// <summary>
	/// アニメーションなし描画
	/// </summary>
	/// <param name="desc"></param>
	static void NormalDraw(const ModelDrawDesc& desc);
	/// <summary>
	/// アニメーションあり描画
	/// </summary>
	/// <param name="desc"></param>
	static void SkinningAnimationDraw(const ModelDrawDesc& desc, Animation* animation);
	/// <summary>
	/// インスタンシング描画
	/// </summary>
	/// <param name="desc"></param>
	/// <param name="instanceNum"></param>
	/// <param name="handle"></param>
	static void InstancedDraw(const ModelDrawDesc& desc, uint32_t instanceNum, D3D12_GPU_DESCRIPTOR_HANDLE handle);

	static void LineDraw(const LineDrawDesc& desc);

};
