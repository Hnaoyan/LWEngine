#include "PlayerAnimation.h"
#include "../../Player.h"
#include "Engine/LwLib/Ease/Ease.h"

void PlayerContext::AnimationManager::Initialize(Player* player)
{
	// プレイヤー
	player_ = player;
	// 胴体の初期化
	bodyTransform_.Initialize();
	bodyTransform_.parent_ = &player->worldTransform_;
	bodyTransform_.UpdateMatrix();

	animParms_.Initialize();
}

void PlayerContext::AnimationManager::Update()
{
	bool isIdle = std::holds_alternative<IdleHorizontal*>(player_->HorizontalManager()->GetVariant());
	
	if (isIdle)
	{
		animParms_.Update();
		// 浮遊を座標に反映
		bodyTransform_.transform_.translate.y = std::sin(animParms_.parameter) * animParms_.width;
	}
	else 
	{
		// タイマーの更新
		animParms_.returnTimer.Update();

		if (animParms_.returnTimer.IsActive())
		{
			bodyTransform_.transform_.translate.y = Ease::Easing(animParms_.start_, 0.0f, animParms_.returnTimer.GetElapsedFrame());
		}
	}

	// 更新
	bodyTransform_.UpdateMatrix();
}

void PlayerContext::AnimationManager::Reset()
{
	animParms_.returnTimer.Start(15.0f);
	animParms_.start_ = bodyTransform_.transform_.translate.y;
}
