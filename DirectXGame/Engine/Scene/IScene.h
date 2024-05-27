#pragma once
#include "../Base/DirectXCommon.h"
#include "../Input/Input.h"
#include "../Camera/ICamera.h"

class IScene
{
public: // 仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ImGuiの処理
	/// </summary>
	virtual void ImGuiDraw() = 0;

	/// <summary>
	/// モデル読み込み
	/// </summary>
	virtual void LoadModel() {};

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	virtual void LoadTexture() {};

	/// <summary>
	/// カメラ更新
	/// </summary>
	virtual void CameraUpdate();

	/// <summary>
	/// コライダー関係の更新
	/// </summary>
	virtual void ColliderUpdate() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IScene() = default;

public: // 取得・設定
	/// <summary>
	/// シーン番号取得
	/// </summary>
	/// <returns></returns>
	int GetSceneNum() { return sceneNum; }

protected:
	/// <summary>
	/// シーン番号
	/// </summary>
	static int sceneNum;

	// DxCommon
	DirectXCommon* dxCommon_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// カメラクラス（ViewProjection
	ICamera camera_;
};

