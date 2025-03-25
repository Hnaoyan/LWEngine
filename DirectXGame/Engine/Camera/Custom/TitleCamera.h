#pragma once
#include "Engine/Camera/Core/ICamera.h"
#include "Engine/3D/WorldTransform.h"

class TitleCamera : public ICamera
{
public:
	void Initialize() override;
	void Update() override;
	void ImGuiDraw() override;
	void InitializeGlobalValue() override;

};
