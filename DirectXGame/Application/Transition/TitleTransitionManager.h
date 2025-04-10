#pragma once
#include "Application/Title/TitleObject.h"
#include "Engine/Object/ObjectPool.h"
#include "Engine/Camera/CameraList.h"

class SceneManager;

class TitleTransitionManager
{
private:
	struct MovePoint {
		Vector3 start, end;
	};

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
	// 遷移の受付関数
	void TransitionExecute();
	// 初期化
	void Initialize(SceneManager* sceneManager);
	// 更新
	void Update();
	// 描画
	void Draw(ICamera* camera, DrawDesc::LightDesc lights);
	// ImGui描画
	void ImGuiDraw();
	// カメラのゲッター
	ICamera* GetCamera() const { return camera_.get(); }
private:
	// タイトルシーン用のオブジェクト
	std::unique_ptr<TitleObject> robotObject_;
	// カメラ
	std::unique_ptr<TitleCamera> camera_;
	// 遷移用のタイマー
	FrameTimer timer_;

	SceneManager* sceneManager_ = nullptr;

	// 座標
	MovePoint cameraPoint_;

	std::unique_ptr<ObjectPool<ICamera, 5>> cameras_pool;

};
