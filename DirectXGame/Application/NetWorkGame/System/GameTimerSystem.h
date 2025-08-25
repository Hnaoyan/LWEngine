#pragma once

/// <summary>
/// ゲームのタイマーを管理するクラス
/// </summary>
class GameTimerSystem
{
public:
	void Initialize();
	void Update();
	void ImGuiDraw();

private:
	// フレーム
	float elapsedFrame_;
	// 秒
	float elapsedSecond_;
};

