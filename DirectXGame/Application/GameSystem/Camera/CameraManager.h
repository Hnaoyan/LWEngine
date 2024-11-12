#pragma once
#include "Engine/Camera/CameraList.h"
#include <memory>
#include <stdfloat>


class CameraManager
{
public:
	CameraManager();
	~CameraManager() = default;

public:
	//void Initialize();
	void Update();

private:
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	
};
