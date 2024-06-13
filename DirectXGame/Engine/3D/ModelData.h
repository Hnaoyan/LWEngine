#pragma once
#include <string>
#include <vector>
#include "CBuffer.h"
#include "../Animation/AnimationData.h"
#include "../Animation/ModelNode.h"
#include "../Animation/SkinCluster.h"

class Model;
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

// モデルの描画に必要な者たち
struct ModelDrawDesc
{
	ModelNode* modelNode;
	WorldTransform* worldTransform;
	ICamera* camera;
	Matrix4x4 localMatrix;
};
