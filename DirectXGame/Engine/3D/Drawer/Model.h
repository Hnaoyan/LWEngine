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
#include "../CBuffer.h"
#include "../WorldTransform.h"
#include "../Mesh.h"
#include "../Material.h"
#include "../ModelData.h"
#include "../Loader.h"
#include "../../Camera/ICamera.h"

class Model
{
private:
	using ModelRegister = Pipeline::ModelRegister;
	using BlendMode = Pipeline::BlendMode;
public:
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ディレクトリパス
	static std::string sDirectoryPath;

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
	static Model* CreateObj(const std::string& modelName, LoadExtension ex);

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

	//void AnimationDraw(const AnimDrawDesc& desc);

public: // アクセッサ
	ModelData* GetModelData() { return &modelData_; }
private:
	// メッシュ
	std::unique_ptr<Mesh> mesh_;
	// マテリアル
	std::unique_ptr<Material> material_;
	// テクスチャ
	uint32_t texture_ = 0u;
private:
	BlendMode blendMode_ = BlendMode::kNormal;

};