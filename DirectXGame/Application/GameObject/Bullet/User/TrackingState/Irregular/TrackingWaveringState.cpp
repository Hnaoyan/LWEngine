#include "TrackingWaveringState.h"
#include "../../../BulletsLists.h"
#include "../StateMachine/BulletStateMachine.h"
#include "Engine/LwLib/LwEngineLib.h"

void TrackingWaveringState::Enter()
{
	// 基準設定
	defaultDirect_ = Vector3::Normalize(bullet_->GetVelocity());
	bullet_->SetAccelerate(Vector3::Zero());
	waveCount_ = 0.0f;

	// 方向の指定
	curveDirect_ = 0;
	curveProcess_ = std::bind(&TrackingWaveringState::LeftCurve, this);
	//curveProcess_ = [&]() {LeftCurve(); };
	curveFrame_ = 60.0f;
	curveTimer_.Start(curveFrame_);

}

void TrackingWaveringState::Update(BulletStateMachine& stateMachine)
{
	// カーブ用のタイマー
	curveTimer_.Update();
	timer_.Update();
	// 右
	if (curveDirect_ == 0) {
		if (curveTimer_.IsEnd()) {
			curveProcess_ = std::bind(&TrackingWaveringState::RightCurve, this);
			//curveProcess_ = [&]() {RightCurve(); };
			curveDirect_ = 1;
			curveTimer_.Start(curveFrame_);
		}
	}
	// 左
	else if (curveDirect_ == 1) {
		if (curveTimer_.IsEnd()) {
			curveProcess_ = [&]() {EndUpdate(); };
			curveDirect_ = 0;
			curveTimer_.Start(curveFrame_);
		}
	}
	else {
		// 三角
		//TrigonometricPattern();
	}

	curveProcess_();

	if (timer_.IsEnd()) {
		stateMachine.RequestState(TrackingState::kStraight);
	}

}

void TrackingWaveringState::Exit()
{
	curveDirect_ = 0;
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

void TrackingWaveringState::RightCurve()
{
	float angle = 45.0f * 2.0f;
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateYMatrix(LwLib::AngleToRadian(angle));
	Vector3 targetVector = Matrix4x4::TransformVector3(defaultDirect_, rotateMatrix);

	bullet_->SetAccelerate(Ease::Easing(defaultDirect_, targetVector, curveTimer_.GetElapsedFrame()));

}

void TrackingWaveringState::LeftCurve()
{
	float angle = -45.0f * 2.0f;
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateYMatrix(LwLib::AngleToRadian(angle));
	Vector3 targetVector = Matrix4x4::TransformVector3(defaultDirect_, rotateMatrix);

	bullet_->SetAccelerate(Ease::Easing(defaultDirect_, targetVector, curveTimer_.GetElapsedFrame()));
}

void TrackingWaveringState::EndUpdate()
{
}
