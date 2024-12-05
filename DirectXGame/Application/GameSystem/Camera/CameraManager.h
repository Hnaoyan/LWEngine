#pragma once
#include "Engine/Camera/CameraList.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include <optional>
#include <memory>
#include <unordered_map>

class GameObjectManager;
class GameSystem;

/// <summary>
/// カメラ管理クラス
/// </summary>
class CameraManager
{
public:
	CameraManager();
	~CameraManager() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="gameManager"></param>
	void Initialize(GameObjectManager* gameManager);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameSystem* gameSystem);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// カメラ変更処理
	/// </summary>
	/// <param name="mode"></param>
	void ChangeCamera(ActiveCameraMode mode);

private:
	// ゲーム中のカメラ切り替わり
	void InGameCameraSwitcher();
	// リプレイのカメラ切り替わり
	void ReplayCameraSwitcher();

	// 入力による切り替え
	void InputSwitch();

public:
	ICamera* GetCamera();
	FollowCamera* GetFollowCamera() { return followCamera_.get(); }

private:
	// オブジェクトマネージャ
	GameObjectManager* gameObjManager_ = nullptr;
	// フレームタイマー
	FrameTimer durationTimer_;

private:
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// 注視カメラ
	std::unique_ptr<FocusCamera> focusCamera_;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// 
	std::unique_ptr<OrbitCamera> orbitCamera_;
	// サイドカメラ
	std::unique_ptr<SideCamera> sideCamera_;
	// カメラのリスト
	std::unordered_map<std::string, std::unique_ptr<ICamera>> cameras_;

	// 変更のリクエスト
	std::optional<ActiveCameraMode> changeRequest_ = std::nullopt;
	// 現在アクティブなカメラ
	ActiveCameraMode activeCamera_ = ActiveCameraMode::kNormal;
};
