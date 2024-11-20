#include "BombEffectPlane.h"
#include "Engine/3D/ModelUtility/ModelManager.h"

void BombEffectPlane::Initialize()
{
	// モデル受け取り
	model_ = ModelManager::GetModel("BombPlane");

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	
}

void BombEffectPlane::Update()
{
}

void BombEffectPlane::Draw(ICamera* camera)
{
	camera;
}
