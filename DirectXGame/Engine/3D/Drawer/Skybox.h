#pragma once
#include "../CBuffer.h"
#include "../ModelData.h"
#include "../Material.h"
#include "../Mesh.h"
#include "../Graphics/GraphicsPSO.h"
#include <string>

class Skybox
{
private:
	// パイプライン
	static GeneralPipeline sPipeline_;

public:
	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static Skybox* CreateSkybox(std::string fileName);
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filePath"></param>
	void Initialize(const std::string& fileName);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(const ModelDrawDesc& desc);

	// モデルデータ
	ModelData modelData_{};
	// メッシュ
	std::unique_ptr<Mesh> mesh_;
	// マテリアル
	std::unique_ptr<Material> material_;

};