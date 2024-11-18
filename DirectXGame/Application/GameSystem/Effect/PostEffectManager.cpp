#include "PostEffectManager.h"

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
