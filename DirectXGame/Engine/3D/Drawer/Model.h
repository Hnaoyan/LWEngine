#pragma once
#include <wrl.h>
#include <vector>
#include <memory>
#include <string>

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "../Graphics/GraphicsPSO.h"
#include "../WorldTransform.h"
#include "../Mesh.h"
#include "../Material.h"
#include "../ModelUtility/ModelData.h"
#include "../ModelUtility/ModelLoader.h"
#include "../../Camera/Core/ICamera.h"

class Animation;

/// <summary>
/// 通常モデルクラス
/// </summary>
class Model
{
private:
	using ModelRegister = Pipeline::ModelRegister;
	using SkinningModelRegister = Pipeline::SkinningModelRegister;
	using BlendMode = Pipeline::BlendMode;
public:
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ディレクトリパス
	static std::string sDirectoryPath;

private:
	// パイプライン
	static GeneralPipeline sPipeline_;

public:
	/// <summary>
	/// 描画前
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画後
	/// </summary>
	static void PostDraw();
	/// <summary>
	// モデル生成
	/// </summary>
	static Model* Create(const std::string& modelName, LoadExtension ex);
	static Model* CreateObj(const std::string& modelName, LoadExtension ex);
	static Model* CreateDefault(const std::string& modelName);

private:
	// モデルのデータ
	ModelData modelData_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& modelName, LoadExtension ex);
	void Initialize(const std::string& modelName);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	void Draw(const ModelDrawDesc& desc);

	void SkinningDraw(const ModelDrawDesc& desc, Animation* animation, uint32_t texture);

	void InstancedDraw(const ModelDrawDesc& desc, uint32_t instanceNum, D3D12_GPU_DESCRIPTOR_HANDLE handle);
	void InstancedDraw(const ModelDrawDesc& desc, uint32_t instanceNum, D3D12_GPU_DESCRIPTOR_HANDLE handle, uint32_t texture);
	
	//void ParticleDraw(const ModelDrawDesc& desc,)
	//void AnimationDraw(const AnimDrawDesc& desc);

public: // アクセッサ
	ModelData* GetModelData() { return &modelData_; }

	ModelData& GetModelDataAdress() { return modelData_; }

	Material* GetMaterial() { return material_.get(); }

	Mesh* GetMesh() { return mesh_.get(); }
private:
	// メッシュ
	std::unique_ptr<Mesh> mesh_;
	// マテリアル
	std::unique_ptr<Material> material_;
	// テクスチャ
	uint32_t texture_ = 0u;
private:
	BlendMode blendMode_ = BlendMode::kAlpha;

};