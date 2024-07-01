#pragma once

class FrameTimer
{
public:
	/// <summary>
	/// 開始
	/// </summary>
	/// <param name="endFrame"></param>
	void Start(float endFrame = 60.0f);
	/// <summary>
	/// 終了（強制終了）
	/// </summary>
	void End();
	/// <summary>
	/// 一時停止
	/// </summary>
	void Stop();
	/// <summary>
	/// 再開
	/// </summary>
	void Resume();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameFactor"></param>
	void Update(float gameFactor = 1.0f);
private:
	/// <summary>
	/// 開始フラグの設定
	/// </summary>
	void StartUp();

public:
	// 経過フレーム（現在フレーム
	float elapsedFrame_ = 0.0f;
	// 終了フレーム
	float endFrame_ = 0.0f;
	// 有効フラグ
	bool isActive_ = false;
	// 終了フラグ
	bool isEnd_ = false;
	// 一時停止フラグ
	bool isPause_ = false;
};