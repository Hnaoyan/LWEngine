#pragma once
#include "IGameSceneState.h"

class GamePlayState : public IGameSceneState
{
public:
	void Enter() override;
	void Update() override;
	void Exit() override;
};
