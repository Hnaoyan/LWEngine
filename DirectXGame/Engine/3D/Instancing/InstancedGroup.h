#pragma once
#include <vector>
#include <memory>

#include "InstancedUnit.h"
#include "../Drawer/Model.h"
#include "../../Base/Utility/DxCreateLib.h"
#include "Engine/Base/CBufferCommon.h"

class InstancedGroup
{
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 256;
	virtual ~InstancedGroup();
protected:
	// リソース
	StructuredBufferContext<CBufferInstancedUnit> buffer_;
	uint32_t srvIndex;

	// Unitの数
	uint32_t unitNum_ = 0u;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	virtual void Draw(ModelDrawDesc desc);
	/// <summary>
	/// リストに登録する関数
	/// </summary>
	/// <param name="position"></param>
	virtual void UnitRegist(const Vector3& position);
	/// <summary>
	/// ImGui
	/// </summary>
	virtual void ImGuiDraw();
	/// <summary>
	/// 削除関数
	/// </summary>
	void DeleteUnit();
public: // アクセッサ

	// ユニットリスト取得
	//std::vector<std::unique_ptr<InstancedUnit>> GetUnitList() { return units_; }

private:
	/// <summary>
	/// インスタンシングのSRV確保など
	/// </summary>
	void CreateData();

protected:
	// ユニットリスト
	std::vector<std::unique_ptr<InstancedUnit>> units_;
	// モデル
	Model* model_ = nullptr;
	// マテリアル
	std::unique_ptr<Material> material_;
	// テクスチャ
	uint32_t texture_ = 0;
};