#pragma once
#include <vector>
#include <memory>

#include "InstancedUnit.h"
#include "../Drawer/Model.h"
#include "../../Base/Utility/DxCreateLib.h"
#include "Engine/Base/CBufferCommon.h"

/// <summary>
/// インスタンシングクラス
/// </summary>
class InstancedGroup
{
public: // エンジン的な部分
	// 最大数
	static const uint32_t kNumInstanceMax = (1024);
	virtual ~InstancedGroup();
protected:
	// リソース
	StructuredBufferContext<CBufferInstancedUnit> buffer_;
	uint32_t srvIndex = 0;

	// Unitの数
	uint32_t unitNum_ = 0u;

	Pipeline::BlendMode blendMode_ = Pipeline::BlendMode::kAlpha;
	// テクスチャ
	uint32_t texture_ = 0;

public: // 描画に使う関数
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return buffer_.GetSRVGPU(); }
	uint32_t GetUnitCount() { return unitNum_; }
	Pipeline::BlendMode GetBlendMode() { return blendMode_; }
	
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
	// モデルを変更できるように
	void SetModel(Model* model) { model_ = model; }
	// 隠せるように
	void SetInvisible(bool isInvisible) { isInvisible_ = isInvisible; }
	// テクスチャ
	void SetTexture(uint32_t texture) { texture_ = texture; }
	// タグ
	void SetTag(const std::string& tag) { tag_ = tag; }

	void SetIsDelete(bool isDelete) { isDelete_ = isDelete; }
	bool IsDelete() const { return isDelete_; }

private:
	/// <summary>
	/// インスタンシングのSRV確保など
	/// </summary>
	void CreateData();

protected: // USER
	// ユニットリスト
	std::vector<std::unique_ptr<InstancedUnit>> units_;
	// モデル
	Model* model_ = nullptr;
	// マテリアル
	std::unique_ptr<Material> material_;
	// 削除フラグ
	bool isDelete_;
	// 隠す用のフラグ
	bool isInvisible_ = false;
	// タグ
	std::string tag_;
};