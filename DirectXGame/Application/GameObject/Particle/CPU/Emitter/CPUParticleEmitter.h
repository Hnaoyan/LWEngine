#pragma once
#include "Engine/3D/Instancing/InstancedGroup.h"

class CPUParticleEmitter : public InstancedGroup
{
public: // 仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc) override;

public:
	void AddParticle(const Vector3& direct);

private:
	// 消えるタイミング
	bool isDead_ = false;
	Vector3 position_{};

};
