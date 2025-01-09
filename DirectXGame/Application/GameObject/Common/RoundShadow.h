#pragma once
#include "Engine/3D/Drawer/Model.h"
#include <memory>

class IGameObject;

class RoundShadow
{
public:
	RoundShadow() {};
	~RoundShadow() = default;
	
	void Initialize(IGameObject* gameObject);
	void Update();
	void Draw(ModelDrawDesc desc);
	void ImGuiDraw();

public:
	void SetDefaultScale(const Vector2& scale) { defaultScale_ = scale; }
	void SetOffset(const float& offset) { offset_ = offset; }
	void SetPosition(const Vector2& position) { targetPosition_ = position; }
private:
	IGameObject* object_ = nullptr;
	Vector2 targetPosition_ = {};	//X-Z座標
	float offset_ = 0.0f;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	std::unique_ptr<Material> material_;
	bool isInvisible_ = false;
	uint32_t texture_ = 0u;
private:
	Vector2 defaultScale_ = {};
};
