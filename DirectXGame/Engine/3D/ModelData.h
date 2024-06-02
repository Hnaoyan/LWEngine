#pragma once
#include <string>
#include <vector>
#include "CBuffer.h"

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
struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
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
	Node rootNode;
};
