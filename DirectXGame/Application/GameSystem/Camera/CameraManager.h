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
	/// <summary>
	/// ゲームのセットアップ
	/// </summary>
	void GameSetUp();
	/// <summary>
	/// チュートリアルのセットアップ
	/// </summary>
	void TutorialSetUp();

private:
	// ゲーム中のカメラ切り替わり
	void InGameCameraSwitcher();
	// リプレイのカメラ切り替わり
	void ReplayCameraSwitcher();
	// 入力による切り替え
	void InputSwitch();

public: // アクセッサ
	ICamera* GetCamera();
	//FollowCamera* GetFollowCamera() { return followCamera_.get(); }
	FollowCamera* GetFollowCamera() { return GetTypeCamera<FollowCamera>("Follow"); }

	template<typename T>
	T* GetTypeCamera(const std::string& tag) { return dynamic_cast<T*>(FindCamera(tag)); }
private:
	ICamera* FindCamera(std::string tag);
private:
	// オブジェクトマネージャ
	GameObjectManager* gameObjManager_ = nullptr;
	// フレームタイマー
	FrameTimer durationTimer_;

private:
	// カメラのリスト
	std::unordered_map<std::string, std::unique_ptr<ICamera>> cameras_;

	// 変更のリクエスト
	std::optional<ActiveCameraMode> changeRequest_ = std::nullopt;
	// 現在アクティブなカメラ
	ActiveCameraMode activeCamera_ = ActiveCameraMode::kNormal;
};
