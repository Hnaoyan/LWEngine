#include "FrameTimer.h"

void FrameTimer::Start(float endFrame)
{
	// 初期化
	StartUp();
	// 終了フレーム設定
	endFrame_ = endFrame;
}

void FrameTimer::End()
{
	// フラグの終了設定
	isEnd_ = true;
	isActive_ = false;
	isPause_ = false;
}

void FrameTimer::Stop()
{
	isPause_ = true;
}

void FrameTimer::Resume()
{
	isPause_ = false;
}

void FrameTimer::Update(float gameFactor)
{
	// ポーズしていない上アクティブの場合更新
	if (isActive_ && !isPause_) {
		// 時間が来たら終了
		if (elapsedFrame_ >= 1.0f) {
			End();
			return;
		}
		// 現在の値計算
		elapsedFrame_ += (1.0f / (gameFactor * endFrame_));
	}
}

void FrameTimer::StartUp()
{
	// フラグ初期化
	isEnd_ = false;
	isActive_ = true;
	// フレーム初期化
	elapsedFrame_ = 0.0f;
}