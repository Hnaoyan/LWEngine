#pragma once

/// <summary>
/// ゲームシーンの状態基底ステート
/// </summary>
class IGameSceneState
{
public:
	/// <summary>
	/// 開始
	/// </summary>
	virtual void Enter() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Exit() = 0;

private:


};
