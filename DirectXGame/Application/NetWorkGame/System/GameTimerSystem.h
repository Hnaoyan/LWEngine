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

	void Start();
	void Pause();
	void Resume();


private:
	// フレーム
	float elapsedFrame_;
	// 秒
	float elapsedSecond_;

	bool isActive_ = false;

};
