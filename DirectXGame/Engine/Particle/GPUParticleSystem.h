#pragma once
#include "Emitter/ParticleEmitter.h"
#include <stdint.h>
#include <vector>

class GPUParticleSystem
{
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 1024;
	// エミッターのリスト
	std::vector<std::unique_ptr<ParticleEmitter>> emitters_;
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
	void CreateEmitter();
private:
	EmitterSphere createData_{};
	Model* model_ = nullptr;
};