#pragma once
#include "Engine/LwLib/LwEnginePaths.h"

class Player;

namespace PlayerContext
{
	class JustDodgeManager
	{
	public:
		JustDodgeManager() {};

		// ジャスト回避開始処理
		void DodgeExcept(const float& frame) {
			dodgeTimer_.Start(frame);
		}
		void InvisibleExcept(const float& frame) {
			invisibleTimer_.Start(frame);
		}
		// 更新
		void Update();
		// ジャスト回避可能状態
		bool IsActive() { return dodgeTimer_.IsActive(); }
		// 無敵状態
		bool IsInvisible() { return invisibleTimer_.IsActive(); }
	private:
		// ドッジ出来る時間
		FrameTimer dodgeTimer_;
		// 無敵時間
		FrameTimer invisibleTimer_;
	};
}

