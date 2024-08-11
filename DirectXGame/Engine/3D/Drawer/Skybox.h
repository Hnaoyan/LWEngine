#pragma once
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

	// シリアルナンバー
	uint32_t serialNum_ = 0;
	// 全体のシリアル
	static uint32_t sSerialNumber_;


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
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
public:
	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3& position) {
		worldTransform_.transform_.translate = position;
	}

	/// <summary>
	/// テクスチャ
	/// </summary>
	/// <returns></returns>
	uint32_t GetTexture() const{ return modelData_.material.textureHandle; }

private:
	// モデルデータ
	ModelData modelData_{};
	// メッシュ
	std::unique_ptr<Mesh> mesh_;
	// マテリアル
	std::unique_ptr<Material> material_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};

};