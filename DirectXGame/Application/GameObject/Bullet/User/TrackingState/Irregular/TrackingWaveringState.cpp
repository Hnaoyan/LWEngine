#include "TrackingWaveringState.h"
#include "../../../BulletsLists.h"
#include "../StateMachine/BulletStateMachine.h"

void TrackingWaveringState::Enter()
{
	// 基準設定
	defaultDirect_ = Vector3::Normalize(bullet_->GetVelocity());
	bullet_->SetAccelerate(Vector3::Zero());
	waveCount_ = 0.0f;

	curveTimer_.Start(15.0f);

}

void TrackingWaveringState::Update(BulletStateMachine& stateMachine)
{
	stateMachine;

	// 三角
	TrigonometricPattern();
}

void TrackingWaveringState::Exit()
{
	waveCount_ = 0.0f;
}

void TrackingWaveringState::TrigonometricPattern()
{
	//Vector3 normalizeVelocity = Vector3::Normalize(velocity_);
	// 横方向のベクトル
	//if (Vector3::Length(sideVector) == 0) {
	//	sideVector = Vector3::Cross(normalizeVelocity, Vector3(0.0f, 0.0f, 1.0f));
	//}
	//Vector3 sideVector = Vector3::Cross(normalizeVelocity, Vector3(0.0f, 1.0f, 0.0f));
	//sideVector.Normalize();

	//// 縦方向のベクトル
	//Vector3 upVector = Vector3::Cross(sideVector, normalizeVelocity);
	//upVector.Normalize();

	//// 揺れの計算
	//float frequency = 1.0f;
	//float swingAmount = 1.0f;
	//waveCount_ += (1.0f / 5.0f);
	//float t = waveCount_ * frequency;
	//Vector2 swing = { std::sinf(t) * swingAmount,std::cosf(t) * swingAmount };
	//Vector3 swing3D = (sideVector * swing.x) + (upVector * swing.y);

	//accelerate_ = normalizeVelocity + swing3D;


	Vector3 velocity = bullet_->GetVelocity().Normalize();

	Vector3 worldUp = Vector3::Up();
	Vector3 side = Vector3::Normalize(Vector3::Cross(worldUp, velocity));
	Vector3 up = Vector3::Normalize(Vector3::Cross(velocity, side));

	float frequency = 0.5f;
	float maxAmount = 10.0f;
	waveCount_ += (1.0f / 2.0f);
	float t = waveCount_ * frequency;
	float swayX = std::sinf(t) * maxAmount;

	Vector3 sway = side * swayX;

	Vector3 newDirect = velocity + sway;
	newDirect = Vector3::Normalize(newDirect);
	bullet_->SetAccelerate(newDirect);
}
