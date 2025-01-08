#pragma once

class IBullet;

class BulletSpeedController
{
public:
	BulletSpeedController(IBullet* bullet) : bullet_(bullet) {}
	
	void Initialize(const float& speed);
	void Update();

	float GetNewSpeed();

private:
	IBullet* bullet_ = nullptr;
	// 基の速度
	float defaultSpeed_ = 0.0f;
	// 現在値
	float currentSpeed_ = 0.0f;
	// 
	float currentframe_ = 0.0f;
};
