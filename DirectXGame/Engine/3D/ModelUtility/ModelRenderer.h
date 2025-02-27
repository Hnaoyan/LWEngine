#pragma once
#include <d3d12.h>
#include "Engine/Animation/Animation.h"
#include "ModelData.h"
#include "../Graphics/GraphicsPSO.h"
#include "Engine/3D/User/MissileTrail.h"
#include "../Drawer/3DDrawers.h"
#include "../User/Trail3D.h"

class InstancedGroup;

/// <summary>
/// モデル描画クラス
/// </summary>
class ModelRenderer
{
private: 
	// 省略パス
	using ModelRegister = Pipeline::ModelRegister;
	using SkinningModelRegister = Pipeline::SkinningModelRegister;
	using BlendMode = Pipeline::BlendMode;

public:
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
private:
	// パイプライン
	static GeneralPipeline sPipeline_;
	// ブレンドモード対応
	static BlendPipeline sBlendPipeline_;

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
	static void NormalDraw(ICamera* camera, const DrawDesc::ModelDesc& modelDesc, const DrawDesc::LightDesc& lightDesc);
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
	static void InstancedDraw(ICamera* camera, const DrawDesc::ModelDesc& modelDesc, const DrawDesc::LightDesc& lightDesc, uint32_t instanceNum, D3D12_GPU_DESCRIPTOR_HANDLE handle);
	static void InstancedDraw(ICamera* camera, const DrawDesc::ModelDesc& modelDesc, const DrawDesc::LightDesc& lightDesc, InstancedGroup* object);
	/// <summary>
	/// 線描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="line"></param>
	static void LineDraw(ICamera* camera, Line3D* line);

	/// <summary>
	/// 軌跡描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="trail"></param>
	static void TrailDraw(ICamera* camera, MissileTrail* trail);

	/// <summary>
	/// 軌跡クラスでの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="trail"></param>
	static void TrailDraw(ICamera* camera, Trail3D* trail);
	static void TriangleDraw(ICamera* camera, Triangle3D* triangle);
};
