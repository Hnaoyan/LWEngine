#pragma once
#include "Application/Title/TitleObject.h"

class SceneManager;

class TitleTransitionManager
{
public:
	/// <summary>
	/// 状態の種類（折り返し前か折り返し後か
	/// </summary>
	enum StateType
	{
		kSetup,
		kReturn,
	};
	void Execute(const float& frame);

	void Initialize(SceneManager* sceneManager);
	void Update();
	void Draw(ICamera* camera, DrawDesc::LightDesc lights);

private:
	// タイトルシーン用のオブジェクト
	std::unique_ptr<TitleObject> robotObject_;
	FrameTimer timer_;
	SceneManager* sceneManager_ = nullptr;
};
