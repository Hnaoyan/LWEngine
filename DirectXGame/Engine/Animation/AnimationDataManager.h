#pragma once
#include "../Utility/Singleton.h"

class AnimationDataManager : public Singleton<AnimationDataManager>
{
public:

	void LoadAnimations();

};
