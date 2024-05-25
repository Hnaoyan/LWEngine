#pragma once
#include <wrl.h>
#include <vector>
#include <string>

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "../Graphics/GraphicsPSO.h"
#include "../CBuffer.h"
#include "../WorldTransform.h"
#include "../../Camera/ICamera.h"
#include "../Mesh.h"


class Model
{
private:
	using ModelRegister = Pipeline::ModelRegister;
public:
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;

public:
	// マテリアル名
	struct MaterialData {
		std::string textureFilename;
	};
	// モデル情報
	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& modelName);

	/// <summary>
	/// モデル生成
	/// </summary>
	Model* CreateObj(const std::string& modelName);
public:
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	void Draw(const WorldTransform& worldTransform, ICamera* camera);

	/// <summary>
	/// 描画前
	/// </summary>
	void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画後
	/// </summary>
	void PostDraw();

private:
	// メッシュ
	std::unique_ptr<Mesh> mesh_;

private:
	Pipeline::BlendMode blendMode_;

};