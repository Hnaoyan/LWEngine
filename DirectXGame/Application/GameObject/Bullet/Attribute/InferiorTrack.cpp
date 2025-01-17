#include "InferiorTrack.h"

void InferiorTrack::Initialize(IBullet* bullet)
{
	// 基底初期化
	ITrackiAttribute::Initialize(bullet);
}

void InferiorTrack::Update()
{
}

Vector3 InferiorTrack::CalculateDirection()
{
	return Vector3();
}
