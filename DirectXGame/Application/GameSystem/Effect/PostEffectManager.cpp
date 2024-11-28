#include "PostEffectManager.h"
#include "Engine/LwLib/LwEnginePaths.h"

PostEffectManager::PostEffectManager()
{
	// ブルーム
	bloomData_.sigma = 1.5f;
	bloomData_.threshold = 0.75f;
	// ビネット
	vignetteData_.color = { 1.0f,0.0f,0.0f };
	vignetteData_.powValue = 0.8f;
	vignetteData_.scale = 16.0f;
}

void PostEffectManager::Update()
{
	// ダメージとダッシュの両方
	if (dashEffect_.timer.IsActive() && damageEffect_.timer.IsActive()) {
		dashEffect_.Update();
		damageEffect_.Update();
	}
	// ダッシュ
	else if (dashEffect_.timer.IsActive())
	{
		dashEffect_.Update();
	}
	// ダメージ
	else if (damageEffect_.timer.IsActive())
	{
		damageEffect_.Update();
	}
}
