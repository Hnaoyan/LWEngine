#pragma once
#include "Emitter/ParticleEmitter.h"
#include <stdint.h>
#include <vector>
#include <unordered_map>

class GPUParticleSystem
{
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 1024;
	// エミッターのリスト
	std::unordered_map<std::string, std::unique_ptr<ParticleEmitter>>  emitters_;

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
	void DeleteEmitter(std::string tag);

private:
	std::string addEmitterName_;
	int nameNum_ = 0;
	EmitterSphere createData_{};
	Model* model_ = nullptr;
};