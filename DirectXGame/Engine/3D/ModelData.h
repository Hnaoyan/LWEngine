#pragma once
#include <string>
#include <vector>
#include "CBuffer.h"

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
};
