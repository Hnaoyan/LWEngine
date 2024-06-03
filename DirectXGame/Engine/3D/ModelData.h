#pragma once
#include <string>
#include <vector>
#include "CBuffer.h"
#include "../Animation/AnimationData.h"

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

// ノード情報
struct ModelNode {
	// クォータニオン
	QuaternionTransform transform;
	// ローカル行列
	Matrix4x4 localMatrix;
	// 名前
	std::string name;
	// 子
	std::vector<ModelNode> children;
};

// マテリアル名
struct MaterialData {
	// テクスチャパス
	std::string textureFilename;
	// テクスチャハンドル
	uint32_t textureHandle;
};
// モデル情報
struct ModelData {
	// 頂点情報
	std::vector<VertexData> vertices;
	// マテリアル
	MaterialData material;
	// ノード
	ModelNode rootNode;
	// アニメーションデータ
	AnimationData animData;
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
};
