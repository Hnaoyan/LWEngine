#pragma once
#include "ITrackAttribute.h"

class InferiorTrack : public ITrackiAttribute
{
public:
	void Initialize(IBullet* bullet) override;
	void Update() override;
private:
	Vector3 CalculateDirection() override;

};
