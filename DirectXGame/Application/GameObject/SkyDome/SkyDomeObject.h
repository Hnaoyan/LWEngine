#pragma once
#include "../IGameObject.h"
#include <memory>
class SkyDomeObject
{
public:
	void Initialize(Model* model);
	void Update();
	void ImGuiDraw();
	void Draw(ModelDrawDesc desc);
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	std::unique_ptr<Material> material_;
};

