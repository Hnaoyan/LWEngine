#pragma once
#include "Engine/LwLib/LwLibLists.h"

class BulletCluster;

class BulletController
{
public:
	BulletController(BulletCluster* cluster) { cluster_ = cluster; };

	//void Update();

private:
	BulletCluster* cluster_ = nullptr;
	FrameTimer straghtTimer_;
};
