#pragma once

class IBullet;

class BulletSpeedController
{
public:
	BulletSpeedController(IBullet* bullet) : bullet_(bullet) {}
	
	void Update();


private:
	IBullet* bullet_ = nullptr;
	// 現在値
	float currentSpeed_ = 0.0f;
	// 初期値
	float initSpeed_ = 0.0f;

};
