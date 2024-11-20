#pragma once
#include "Engine/3D/Drawer/Model.h"

class ICamera;

class BombEffectPlane
{
public:
	void Initialize();
	void Update();
	void Draw(ICamera* camera);

public:
	// 座標設定
	void SetPosition(const Vector3& position) { worldTransform_.transform_.translate = position; }

private:
	WorldTransform worldTransform_{};
	Model* model_ = nullptr;

};
