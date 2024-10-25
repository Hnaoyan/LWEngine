#pragma once

class IBullet;

/// <summary>
/// 追尾弾の基底ステート
/// </summary>
class ITrackingState
{
public:
	ITrackingState() {};
	virtual ~ITrackingState() = default;

	// 初期化
	virtual void Initialize() {};
	// 開始
	virtual void Enter() {};
	// 更新
	virtual void Update() = 0;
	// 終了
	virtual void Exit() {};

public:
	void SetBullet(IBullet* bullet) { bullet_ = bullet; }

protected:
	IBullet* bullet_ = nullptr;

};
