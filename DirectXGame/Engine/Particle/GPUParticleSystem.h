#pragma once
#include "Emitter/GPUParticleEmitter.h"
#include <stdint.h>
#include <vector>
#include <unordered_map>

class GPUParticleManager
{
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 1024;
	// エミッターのリスト
	std::unordered_map<std::string, std::unique_ptr<GPUParticleEmitter>>  emitters_;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GPUParticleManager();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(ICamera* camera);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// エミッターの作成
	/// </summary>
	void CreateEmitter(std::string tag);
	/// <summary>
	/// エミッター作成（インスタンスをそのまま引数で
	/// </summary>
	/// <param name="instance"></param>
	/// <param name="tag"></param>
	void CreateEmitter(std::unique_ptr<GPUParticleEmitter> instance, std::string tag);
	/// <summary>
	/// エミッターの削除
	/// </summary>
	/// <param name="tag"></param>
	void DeleteEmitter(std::string tag);
	void DeleteEmitter(GPUParticleEmitter* emitter);
	/// <summary>
	/// リストからポインタの検索
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	GPUParticleEmitter* FindEmitter(std::string tag);

	/// <summary>
	/// データの初期化
	/// </summary>
	void DataReset();

private:
	bool isCreate_ = false;	// 生成フラグ
	bool isDelete_ = false;	// 削除フラグ
	std::string addEmitterName_;	// 追加するエミッターの名前
	int nameNum_ = 0;	// 名前の番号
	EmitterSphere createData_{};	// 作成データ	
	Model* model_ = nullptr;	// モデル
};