#include "JustDudgeManager.h"

void PlayerContext::JustDodgeManager::Update()
{
	// 回避タイマー
	dodgeTimer_.Update();
	// 無敵タイマー
	invisibleTimer_.Update();
}
