#pragma once
#include "../Base/DirectXCommon.h"
#include "../Input/Input.h"
#include "../Camera/Core/ICamera.h"

class SceneManager;

/// <summary>
/// シーンのインタフェース
/// </summary>
class IScene
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IScene() = default;

public: // フレーム関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// GPUの更新処理
	/// </summary>
	virtual void GPUUpdate() {};
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// UI描画
	/// </summary>
	virtual void UIDraw() = 0;
	/// <summary>
	/// ImGuiの処理
	/// </summary>
	virtual void ImGuiDraw() = 0;
public:	// ロード関数
	/// <summary>
	/// モデル読み込み
	/// </summary>
	virtual void LoadModel() {};
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	virtual void LoadTexture() {};

	void LoadResource() {
		LoadModel();
		LoadTexture();
		isSceneReady_ = true;
	}


public: // 取得・設定
	/// <summary>
	/// シーン番号取得
	/// </summary>
	/// <returns></returns>
	int GetSceneNum() { return sceneNum; }

	/// <summary>
	/// マネージャー設定
	/// </summary>
	/// <param name="sceneM"></param>
	void SetSceneManager(SceneManager* sceneM) { sceneManager_ = sceneM; }

	bool GetSceneReady() { return isSceneReady_; }
protected:
	/// <summary>
	/// シーン番号
	/// </summary>
	static int sceneNum;

	// シーンの初期化を完了したかを確認する
	bool isSceneReady_;
	// シーンマネージャー
	SceneManager* sceneManager_ = nullptr;
	// DxCommon
	DirectXCommon* dxCommon_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// カメラクラス（ViewProjection
	ICamera camera_;
};

