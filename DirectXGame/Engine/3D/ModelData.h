#pragma once
#include <string>
#include <vector>
#include "Engine/Base/CBufferCommon.h"
#include "../Animation/AnimationData.h"
#include "../Animation/ModelNode.h"
#include "../Animation/SkinCluster.h"

#include "../Light/LightLists.h"

class Model;
class Mesh;
class Material;
class WorldTransform;
class ICamera;

/// <summary>
/// 読み込み拡張子
/// </summary>
enum class LoadExtension : uint16_t
{
	kObj,	// .obj
	kGltf,	// .gltf

	kEndCount,	// 最後
};

/// <summary>
/// モデルの種類
/// </summary>
enum class ModelType : uint32_t
{
	kNormal,
	kSkinning,
};


// マテリアル名
struct MaterialData {
	// テクスチャパス
	std::string textureFilename;
	// テクスチャハンドル
	uint32_t textureHandle;
};

// モデル情報
class ModelData {
public:
	// 頂点情報
	std::vector<VertexData> vertices;
	// マテリアル
	MaterialData material;
	// ノード
	ModelNode rootNode;
	// アニメーションデータ
	AnimationData animData;
	// Index情報
	std::vector<uint32_t> indices;

	// SkinCluster用のデータ
	std::map<std::string, JointWeightData> skinClusterData;

};

// アニメーションの際に必要なやつ
struct AnimDrawDesc
{
	//Model* model;
	WorldTransform* worldTransform;
	ICamera* camera;
};

struct LineDrawDesc {
	ICamera* camera = nullptr;

};

// モデルの描画に必要な者たち
struct ModelDrawDesc
{
	// ワールドトランスフォーム
	WorldTransform* worldTransform;
	// メッシュ
	Mesh* mesh = nullptr;
	// マテリアル
	Material* material = nullptr;
	// カメラ
	ICamera* camera = nullptr;
	// テクスチャ
	uint32_t texture = 100;
	// モデルデータ
	ModelData* modelData;
	// 平行光源
	DirectionalLight* directionalLight = nullptr;
	// 方向光源
	SpotLight* spotLight = nullptr;
	// 点光源
	PointLight* pointLight = nullptr;

	/// <summary>
	/// カメラとライトの設定
	/// </summary>
	/// <param name="desc"></param>
	/// <returns></returns>
	static ModelDrawDesc SetDesc(const ModelDrawDesc& desc) {
		ModelDrawDesc result{};
		// カメラ
		result.camera = desc.camera;
		// ライト
		result.directionalLight = desc.directionalLight;
		result.pointLight = desc.pointLight;
		result.spotLight = desc.spotLight;
		return result;
	}

};

/// <summary>
/// 今後作る可能性のある設計図
/// </summary>
namespace Modeler {
	class Drawer : public Singleton<Drawer> {

	};
	struct Desc {

	};
	class Loader {

	};

}
