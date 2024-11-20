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

	if (dashEffect_.isActive) {
		dashEffect_.timer.Update();
		dashEffect_.data.blurWidth = Ease::Easing(dashEffect_.maxWidth, 0.0f, dashEffect_.timer.GetElapsedFrame());
	}

}
