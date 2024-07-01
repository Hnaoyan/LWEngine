#pragma once
#include "SampleBullet.h"
#include <vector>

class SampleBulletManager
{
public:
	struct GenerateData {
		Vector3 position;
		Vector3 velocity;
	};

public:
	void Initialize(Model* model);
	void Update();
	void Draw(ModelDrawDesc desc);

	void AddBullet(const GenerateData& data);

private:
	// 全部のモデル
	Model* model_ = nullptr;
	// 弾のリスト
	std::vector<std::unique_ptr<SampleBullet>> bullets_;

};
