#pragma once
/// <summary>
/// 2D3Dゲーム用
/// </summary>
class CameraManager2D
{
public:
	CameraManager2D();
	~CameraManager2D() = default;

public:
	void Initialize();
	void Update();
	void ImGuiDraw();
};

