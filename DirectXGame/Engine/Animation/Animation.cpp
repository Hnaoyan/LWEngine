#include "Animation.h"
#include "../System/DeltaTime.h"

void Animation::UpdateAnimation()
{
	animationTime_ += kDeltaTime;
	animationTime_ = std::fmod(animationTime_, animData_.duration);
	//NodeAnimation& nodeAnim = animData_.nodeAnimations[modelData_->rootNode.name];

}
